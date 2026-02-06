/* =========================
   ESCENA / CÁMARA / RENDER
========================= */
const scene = new THREE.Scene();

let width = window.innerWidth;
let height = window.innerHeight;

const camera = new THREE.OrthographicCamera(
  width / -2,
  width / 2,
  height / 2,
  height / -2,
  0.1,
  1000
);
camera.position.z = 10;

const renderer = new THREE.WebGLRenderer({ antialias: true });
renderer.setSize(width, height);
renderer.setClearColor(0x000000);
document.body.appendChild(renderer.domElement);

/* =========================
   PARÁMETROS GLOBALES
========================= */
let droneSpeed = 0.07;
let droneCount = 800;
let baseRadius = 120;
let baseOrbitSpeed = 0.015;
let collisionsEnabled = true;
let globalPersonalSpace = 8;
let globalSeparationForce = 0.5;
let swarm;

/* =========================
   CANVAS INPUT
========================= */
const inputCanvas = document.getElementById("inputCanvas");
const inputCtx = inputCanvas.getContext("2d");

// Ajustar tamaño del canvas según pantalla
function resizeInputCanvas() {
  const maxWidth = Math.min(800, window.innerWidth - 40);
  inputCanvas.width = maxWidth;
  inputCanvas.height = 200;
}
resizeInputCanvas();

function clearInputCanvas() {
  inputCtx.clearRect(0, 0, inputCanvas.width, inputCanvas.height);
}

/* =========================
   TEXTO / CANVAS → PUNTOS
========================= */
function canvasToPoints() {
  const img = inputCtx.getImageData(0, 0, inputCanvas.width, inputCanvas.height);
  
  // Contar píxeles activos para densidad adaptativa
  let activePixels = 0;
  for (let i = 3; i < img.data.length; i += 4) {
    if (img.data[i] > 128) activePixels++;
  }
  
  // Densidad adaptativa
  let step = 6;
  if (activePixels < 6000) step = 3;
  if (activePixels < 3000) step = 2;
  
  const points = [];
  
  for (let y = 0; y < img.height; y += step) {
    for (let x = 0; x < img.width; x += step) {
      const i = (y * img.width + x) * 4;
      if (img.data[i + 3] > 128) {
        points.push({
          x: x - inputCanvas.width / 2,
          y: inputCanvas.height / 2 - y
        });
      }
    }
  }
  return points;
}

function textToPoints(text) {
  clearInputCanvas();
  
  const lines = text.split("\n").filter(line => line.trim() !== "");
  if (lines.length === 0) return [];
  
  const maxWidth = inputCanvas.width * 0.9;
  const maxHeight = inputCanvas.height * 0.9;
  
  let fontSize = 140;
  const lineSpacing = 1.2;
  
  inputCtx.textAlign = "center";
  inputCtx.textBaseline = "middle";
  inputCtx.fillStyle = "white";
  
  // Ajustar tamaño para que TODAS las líneas quepan
  while (fontSize > 20) {
    inputCtx.font = `bold ${fontSize}px Arial`;
    
    const widestLine = Math.max(
      ...lines.map(l => inputCtx.measureText(l).width)
    );
    
    const totalHeight = lines.length * fontSize * lineSpacing;
    
    if (widestLine <= maxWidth && totalHeight <= maxHeight) break;
    
    fontSize--;
  }
  
  // Centrado vertical real
  const startY = inputCanvas.height / 2 - ((lines.length - 1) * fontSize * lineSpacing) / 2;
  
  lines.forEach((line, i) => {
    const y = startY + i * fontSize * lineSpacing;
    
    // Borde para mejor visibilidad
    inputCtx.lineWidth = 2;
    inputCtx.strokeStyle = "white";
    inputCtx.strokeText(line, inputCanvas.width / 2, y);
    inputCtx.fillText(line, inputCanvas.width / 2, y);
  });
  
  return canvasToPoints();
}

/* =========================
   DIBUJO CON MOUSE
========================= */
let drawing = false;
let lastX = 0;
let lastY = 0;

inputCanvas.addEventListener("mousedown", e => {
  drawing = true;
  const r = inputCanvas.getBoundingClientRect();
  lastX = e.clientX - r.left;
  lastY = e.clientY - r.top;
  
  // Dibujar un punto inicial
  inputCtx.beginPath();
  inputCtx.arc(lastX, lastY, 3, 0, Math.PI * 2);
  inputCtx.fillStyle = "white";
  inputCtx.fill();
});

["mouseup", "mouseleave"].forEach(ev =>
  inputCanvas.addEventListener(ev, () => drawing = false)
);

inputCanvas.addEventListener("mousemove", e => {
  if (!drawing) return;
  
  const r = inputCanvas.getBoundingClientRect();
  const x = e.clientX - r.left;
  const y = e.clientY - r.top;
  
  inputCtx.strokeStyle = "white";
  inputCtx.lineWidth = 6;
  inputCtx.lineCap = "round";
  inputCtx.lineJoin = "round";
  
  inputCtx.beginPath();
  inputCtx.moveTo(lastX, lastY);
  inputCtx.lineTo(x, y);
  inputCtx.stroke();
  
  lastX = x;
  lastY = y;
});

/* =========================
   DRONE (CON SISTEMA DE COLISIONES)
========================= */
class Drone {
  constructor(x, y) {
    const g = new THREE.CircleGeometry(2, 10);
    const m = new THREE.MeshBasicMaterial({ color: 0xffffff });
    this.mesh = new THREE.Mesh(g, m);
    this.mesh.position.set(x, y, 0);
    this.target = new THREE.Vector3(x, y, 0);
    this.isOrbiting = false;
    this.originalColor = 0xffffff;
    this.orbitColor = 0x00ffff;
    this.colorTransition = 0;
    this.pulsePhase = Math.random() * Math.PI * 2;
    
    // Sistema de colisiones
    this.velocity = new THREE.Vector3(
      (Math.random() - 0.5) * 0.2,
      (Math.random() - 0.5) * 0.2,
      0
    );
    this.radius = 3; // Radio de colisión
    this.pushForce = 0.3;
    this.maxSpeed = 1.5;
    this.personalSpace = globalPersonalSpace;
    this.maxForce = globalSeparationForce;
    
    // Para comportamiento de bandada 
    this.alignmentFactor = 0.1;
    this.cohesionFactor = 0.05;
  }
  
  update(neighbors = []) {
    // Resolver colisiones primero
    if (collisionsEnabled) {
      this.resolveCollisions(neighbors);
      
      // Comportamiento de bandada solo para drones orbitales
      if (this.isOrbiting) {
        this.applyFlocking(neighbors);
      }
    }
    
    // Movimiento hacia el objetivo
    this.moveToTarget();
    
    // Aplicar velocidad
    this.mesh.position.x += this.velocity.x;
    this.mesh.position.y += this.velocity.y;
    
    // Aplicar fricción suave
    this.velocity.multiplyScalar(0.97);
    
    // Transición de color
    if (this.isOrbiting) {
      this.colorTransition = Math.min(this.colorTransition + 0.05, 1);
    } else {
      this.colorTransition = Math.max(this.colorTransition - 0.05, 0);
    }
    
    // Interpolar color
    const color = new THREE.Color();
    color.lerpColors(
      new THREE.Color(this.originalColor),
      new THREE.Color(this.orbitColor),
      this.colorTransition
    );
    this.mesh.material.color.copy(color);
    
    // Efecto de pulso para drones orbitales
    if (this.isOrbiting) {
      this.pulsePhase += 0.05;
      const pulse = Math.sin(this.pulsePhase) * 0.2 + 1;
      this.mesh.scale.setScalar(pulse);
      
      // Rotación ligera según dirección
      if (this.velocity.length() > 0.1) {
        this.mesh.rotation.z += 0.05;
      }
    } else {
      this.mesh.scale.setScalar(1);
      this.mesh.rotation.z = 0;
    }
    
    // Mantener dentro de límites
    this.stayInBounds();
  }
  
  resolveCollisions(neighbors) {
    let totalPushX = 0;
    let totalPushY = 0;
    let collisionCount = 0;
    
    for (const neighbor of neighbors) {
      if (neighbor === this) continue;
      
      const dx = this.mesh.position.x - neighbor.mesh.position.x;
      const dy = this.mesh.position.y - neighbor.mesh.position.y;
      const distance = Math.sqrt(dx * dx + dy * dy);
      
      const minDistance = this.personalSpace;
      
      if (distance < minDistance && distance > 0) {
        // Calcular fuerza de repulsión
        const overlap = minDistance - distance;
        const pushStrength = (overlap / minDistance) * this.pushForce;
        
        totalPushX += (dx / distance) * pushStrength;
        totalPushY += (dy / distance) * pushStrength;
        collisionCount++;
      }
    }
    
    // Aplicar fuerza de repulsión promedio
    if (collisionCount > 0) {
      this.velocity.x += totalPushX / collisionCount;
      this.velocity.y += totalPushY / collisionCount;
    }
  }
  
  applyFlocking(neighbors) {
    if (neighbors.length === 0) return;
    
    let avgVelocityX = 0;
    let avgVelocityY = 0;
    let avgPositionX = 0;
    let avgPositionY = 0;
    let flockCount = 0;
    
    for (const neighbor of neighbors) {
      if (neighbor.isOrbiting && neighbor !== this) {
        avgVelocityX += neighbor.velocity.x;
        avgVelocityY += neighbor.velocity.y;
        avgPositionX += neighbor.mesh.position.x;
        avgPositionY += neighbor.mesh.position.y;
        flockCount++;
      }
    }
    
    if (flockCount > 0) {
      // Alineación (seguir dirección promedio)
      avgVelocityX /= flockCount;
      avgVelocityY /= flockCount;
      const alignX = (avgVelocityX - this.velocity.x) * this.alignmentFactor;
      const alignY = (avgVelocityY - this.velocity.y) * this.alignmentFactor;
      
      // Cohesión (acercarse al centro del grupo)
      avgPositionX /= flockCount;
      avgPositionY /= flockCount;
      const toCenterX = avgPositionX - this.mesh.position.x;
      const toCenterY = avgPositionY - this.mesh.position.y;
      const distToCenter = Math.sqrt(toCenterX * toCenterX + toCenterY * toCenterY);
      
      if (distToCenter > 0) {
        const cohesionX = (toCenterX / distToCenter - this.velocity.x) * this.cohesionFactor;
        const cohesionY = (toCenterY / distToCenter - this.velocity.y) * this.cohesionFactor;
        
        this.velocity.x += alignX + cohesionX;
        this.velocity.y += alignY + cohesionY;
      }
    }
  }
  
  moveToTarget() {
    const dx = this.target.x - this.mesh.position.x;
    const dy = this.target.y - this.mesh.position.y;
    const distance = Math.sqrt(dx * dx + dy * dy);
    
    if (distance > 0.5) { // Umbral mínimo
      // Velocidad adaptativa: más lento cerca del objetivo
      const targetSpeed = Math.min(this.maxSpeed, distance * 0.05);
      const moveX = (dx / distance) * targetSpeed;
      const moveY = (dy / distance) * targetSpeed;
      
      this.velocity.x += moveX * droneSpeed * 10;
      this.velocity.y += moveY * droneSpeed * 10;
    }
    
    // Limitar velocidad máxima
    const currentSpeed = Math.sqrt(this.velocity.x * this.velocity.x + this.velocity.y * this.velocity.y);
    if (currentSpeed > this.maxSpeed) {
      this.velocity.x = (this.velocity.x / currentSpeed) * this.maxSpeed;
      this.velocity.y = (this.velocity.y / currentSpeed) * this.maxSpeed;
    }
  }
  
  stayInBounds() {
    const margin = 100;
    const turnFactor = 0.5;
    
    if (this.mesh.position.x < -width/2 + margin) {
      this.velocity.x += turnFactor;
    } else if (this.mesh.position.x > width/2 - margin) {
      this.velocity.x -= turnFactor;
    }
    
    if (this.mesh.position.y < -height/2 + margin) {
      this.velocity.y += turnFactor;
    } else if (this.mesh.position.y > height/2 - margin) {
      this.velocity.y -= turnFactor;
    }
  }
}


/* =========================
   SWARM (CON SISTEMA DE VECINOS OPTIMIZADO)
========================= */
class Swarm {
  constructor(count) {
    this.drones = [];
    for (let i = 0; i < count; i++) {
      const x = Math.random() * width - width / 2;
      const y = Math.random() * height - height / 2;
      const d = new Drone(x, y);
      this.drones.push(d);
      scene.add(d.mesh);
    }
    this.orbitDrones = [];
    this.orbitCenter = new THREE.Vector3(0, 0, 0);
    this.orbitTime = 0;
    this.currentPoints = [];
    this.orbitalCount = 0;
    
    // Optimización para búsqueda de vecinos
    this.grid = new Map();
    this.cellSize = globalPersonalSpace * 3; // Tamaño de celda basado en espacio personal
    
    this.updateInfo();
  }
  
  dispose() {
    this.drones.forEach(d => scene.remove(d.mesh));
    this.drones = [];
    this.orbitDrones = [];
  }
  
  // Actualizar grid espacial para búsqueda rápida de vecinos
  updateGrid() {
    this.grid.clear();
    
    for (const drone of this.drones) {
      const cellX = Math.floor(drone.mesh.position.x / this.cellSize);
      const cellY = Math.floor(drone.mesh.position.y / this.cellSize);
      const key = `${cellX},${cellY}`;
      
      if (!this.grid.has(key)) {
        this.grid.set(key, []);
      }
      this.grid.get(key).push(drone);
    }
  }
  
  // Encontrar vecinos cercanos de forma optimizada
  getNeighbors(drone) {
    const neighbors = [];
    const cellX = Math.floor(drone.mesh.position.x / this.cellSize);
    const cellY = Math.floor(drone.mesh.position.y / this.cellSize);
    const searchRadius = Math.ceil(drone.personalSpace * 2 / this.cellSize);
    
    // Buscar en celdas adyacentes
    for (let dx = -searchRadius; dx <= searchRadius; dx++) {
      for (let dy = -searchRadius; dy <= searchRadius; dy++) {
        const key = `${cellX + dx},${cellY + dy}`;
        const cellDrones = this.grid.get(key);
        
        if (cellDrones) {
          for (const otherDrone of cellDrones) {
            if (otherDrone !== drone) {
              const distance = drone.mesh.position.distanceTo(otherDrone.mesh.position);
              if (distance < drone.personalSpace * 2) {
                neighbors.push(otherDrone);
              }
            }
          }
        }
      }
    }
    
    return neighbors;
  }
  
  setFormation(points) {
    this.currentPoints = [...points];
    
    // Resetear todos los drones
    this.drones.forEach(d => {
      d.isOrbiting = false;
      d.velocity.set(0, 0, 0); // Resetear velocidad
    });
    
    this.orbitDrones = [];
    
    if (points.length === 0) {
      // Si no hay puntos, todos van a órbita
      this.drones.forEach((drone, i) => {
        drone.isOrbiting = true;
        this.orbitDrones.push(i);
      });
    } else {
      // Distribuir drones uniformemente
      const dronesPerPoint = Math.max(1, Math.floor(this.drones.length / points.length));
      
      for (let i = 0; i < this.drones.length; i++) {
        const drone = this.drones[i];
        
        if (i < points.length * dronesPerPoint) {
          // Asignar a punto de formación
          const pointIndex = Math.floor(i / dronesPerPoint) % points.length;
          const p = points[pointIndex];
          drone.target.set(p.x, p.y, 0);
          drone.isOrbiting = false;
        } else {
          // Este drone será orbital
          drone.isOrbiting = true;
          this.orbitDrones.push(i);
        }
      }
    }
    
    // Calcular centro para órbita
    if (points.length > 0) {
      const sum = points.reduce((acc, p) => {
        acc.x += p.x;
        acc.y += p.y;
        return acc;
      }, { x: 0, y: 0 });
      
      this.orbitCenter.set(
        sum.x / points.length,
        sum.y / points.length,
        0
      );
    } else {
      this.orbitCenter.set(0, 0, 0);
    }
    
    this.orbitalCount = this.orbitDrones.length;
    this.updateInfo();
  }
  
  update() {
    // Incrementar tiempo para animación orbital
    this.orbitTime += 0.05;
    
    // Actualizar grid espacial (para colisiones)
    this.updateGrid();
    
    // Calcular posiciones objetivo primero
    for (let i = 0; i < this.drones.length; i++) {
      const d = this.drones[i];
      
      if (d.isOrbiting) {
        const orbitIndex = this.orbitDrones.indexOf(i);
        
        // Usar las variables globales baseRadius y baseOrbitSpeed
        const radiusVariation = (orbitIndex % 8) * 25;
        const orbitRadius = baseRadius + radiusVariation;
        
        const speedVariation = (orbitIndex % 13) * 0.003;
        const orbitSpeed = baseOrbitSpeed + speedVariation;
        
        // Ángulo con desfase único
        const angle = this.orbitTime * orbitSpeed + orbitIndex * 0.15;
        
        // Efecto de elipse y wobble
        const ellipseRatio = 0.7 + Math.sin(orbitIndex * 0.5) * 0.3;
        const wobbleX = Math.sin(this.orbitTime * 0.2 + orbitIndex) * 10;
        const wobbleY = Math.cos(this.orbitTime * 0.25 + orbitIndex) * 10;
        
        d.target.x = this.orbitCenter.x + Math.cos(angle) * orbitRadius * ellipseRatio + wobbleX;
        d.target.y = this.orbitCenter.y + Math.sin(angle) * orbitRadius + wobbleY;
      }
    }
    
    // Actualizar cada drone con sus vecinos
    for (let i = 0; i < this.drones.length; i++) {
      const d = this.drones[i];
      const neighbors = this.getNeighbors(d);
      d.update(neighbors);
    }
  }
  
  updateInfo() {
    const infoText = document.getElementById("infoText");
    if (infoText) {
      const activeDrones = this.currentPoints.length > 0 
        ? Math.min(this.drones.length, this.currentPoints.length)
        : 0;
      infoText.textContent = `${this.drones.length} drones | ${this.orbitalCount} orbitales | ${activeDrones} en formación`;
    }
  }
  
  // Método para actualizar parámetros de colisiones en todos los drones
  updateCollisionParams() {
    this.drones.forEach(d => {
      d.personalSpace = globalPersonalSpace;
      d.maxForce = globalSeparationForce;
      d.pushForce = globalSeparationForce * 0.6;
      // Actualizar tamaño de celda para optimización
      this.cellSize = globalPersonalSpace * 3;
    });
  }
}

/* =========================
   UI CONTROLS
========================= */
// Actualizar valores mostrados
function updateSliderValues() {
  document.getElementById("speedValue").textContent = droneSpeed.toFixed(2);
  document.getElementById("radiusValue").textContent = baseRadius;
  document.getElementById("orbitSpeedValue").textContent = baseOrbitSpeed.toFixed(3);
  document.getElementById("personalSpaceValue").textContent = globalPersonalSpace;
  document.getElementById("separationForceValue").textContent = globalSeparationForce.toFixed(1);
}

// Texto
document.getElementById("textBtn").onclick = () => {
  const t = document.getElementById("textInput").value;
  if (t.trim()) {
    swarm.setFormation(textToPoints(t));
  }
};

// Dibujo
document.getElementById("drawBtn").onclick = () => {
  const p = canvasToPoints();
  if (p.length > 0) {
    swarm.setFormation(p);
  } else {
    // Si no hay dibujo, usar texto por defecto
    const t = document.getElementById("textInput").value || "HOLA";
    swarm.setFormation(textToPoints(t));
  }
};

// Limpiar
document.getElementById("clearBtn").onclick = () => {
  clearInputCanvas();
  swarm.setFormation([]);
};

// Controles de velocidad
document.getElementById("speedRange").oninput = e => {
  droneSpeed = parseFloat(e.target.value);
  updateSliderValues();
};

// Controles de órbita
document.getElementById("orbitRadius").oninput = e => {
  baseRadius = parseFloat(e.target.value);
  updateSliderValues();
};

document.getElementById("orbitSpeed").oninput = e => {
  baseOrbitSpeed = parseFloat(e.target.value);
  updateSliderValues();
};

// Controles de colisiones
document.getElementById("collisionsToggle").onchange = e => {
  collisionsEnabled = e.target.checked;
};

document.getElementById("personalSpaceRange").oninput = e => {
  globalPersonalSpace = parseInt(e.target.value);
  updateSliderValues();
  if (swarm) swarm.updateCollisionParams();
};

document.getElementById("separationForceRange").oninput = e => {
  globalSeparationForce = parseFloat(e.target.value);
  updateSliderValues();
  if (swarm) swarm.updateCollisionParams();
};

// Aplicar nuevo número de drones
document.getElementById("applyDrones").onclick = () => {
  const newCount = parseInt(document.getElementById("droneCount").value);
  if (newCount > 0 && newCount <= 3000) {
    droneCount = newCount;
    const oldPoints = swarm.currentPoints;
    swarm.dispose();
    swarm = new Swarm(droneCount);
    
    // Restaurar formación si existía
    if (oldPoints && oldPoints.length > 0) {
      setTimeout(() => swarm.setFormation(oldPoints), 100);
    }
    
    // Aplicar parámetros de colisiones
    swarm.updateCollisionParams();
  }
};

// Botón aleatorio
document.getElementById("randomBtn").onclick = () => {
  swarm.drones.forEach(d => {
    d.target.set(
      Math.random() * width - width / 2,
      Math.random() * height - height / 2,
      0
    );
    d.isOrbiting = false;
    d.velocity.set(0, 0, 0);
  });
  swarm.orbitDrones = [];
  swarm.orbitalCount = 0;
  swarm.updateInfo();
};

// Botón centrar
document.getElementById("centerBtn").onclick = () => {
  swarm.drones.forEach(d => {
    d.target.set(0, 0, 0);
    d.isOrbiting = false;
    d.velocity.set(0, 0, 0);
  });
  swarm.orbitDrones = [];
  swarm.orbitalCount = 0;
  swarm.orbitCenter.set(0, 0, 0);
  swarm.updateInfo();
};

/* =========================
   INIT
========================= */
swarm = new Swarm(droneCount);
swarm.updateCollisionParams();

// Formar texto inicial
setTimeout(() => {
  const initialText = document.getElementById("textInput").value;
  if (initialText.trim()) {
    swarm.setFormation(textToPoints(initialText));
  }
}, 500);

updateSliderValues();

/* =========================
   ANIMATION LOOP
========================= */
let lastTime = 0;
const fixedTimeStep = 1000 / 60; // 60 FPS fijos para física

function animate(currentTime) {
  requestAnimationFrame(animate);
  
  // Física con paso de tiempo fijo para consistencia
  const deltaTime = currentTime - lastTime || 0;
  lastTime = currentTime;
  
  // Actualizar swarm (que incluye física de colisiones)
  swarm.update();
  
  renderer.render(scene, camera);
}
animate();

/* =========================
   RESIZE HANDLING
========================= */
window.addEventListener("resize", () => {
  width = window.innerWidth;
  height = window.innerHeight;
  
  camera.left = width / -2;
  camera.right = width / 2;
  camera.top = height / 2;
  camera.bottom = height / -2;
  camera.updateProjectionMatrix();
  
  renderer.setSize(width, height);
  
  // Redimensionar canvas de input
  resizeInputCanvas();
  
  // Redibujar texto si existe
  if (swarm.currentPoints.length > 0) {
    const textarea = document.getElementById("textInput");
    if (textarea.value.trim()) {
      clearInputCanvas();
      const points = textToPoints(textarea.value);
      swarm.setFormation(points);
    }
  }
});

/* =========================
   KEYBOARD SHORTCUTS
========================= */
document.addEventListener("keydown", e => {
  if (e.target.tagName === "TEXTAREA" || e.target.tagName === "INPUT") return;
  
  switch(e.key.toLowerCase()) {
    case " ":
      e.preventDefault();
      clearInputCanvas();
      swarm.setFormation([]);
      break;
    case "r":
      document.getElementById("randomBtn").click();
      break;
    case "c":
      document.getElementById("centerBtn").click();
      break;
    case "t":
      document.getElementById("textBtn").click();
      break;
    case "d":
      document.getElementById("drawBtn").click();
      break;
    case "escape":
      document.getElementById("clearBtn").click();
      break;
    case "+":
    case "=":
      baseRadius = Math.min(baseRadius + 10, 300);
      document.getElementById("orbitRadius").value = baseRadius;
      updateSliderValues();
      break;
    case "-":
      baseRadius = Math.max(baseRadius - 10, 50);
      document.getElementById("orbitRadius").value = baseRadius;
      updateSliderValues();
      break;
    case "x":
      // Toggle colisiones
      collisionsEnabled = !collisionsEnabled;
      document.getElementById("collisionsToggle").checked = collisionsEnabled;
      break;
  }
});

/* =========================
   DEBUG VISUAL (OPCIONAL)
========================= */
// Para ver áreas de colisión (activar/desactivar con 'v')
let debugMode = false;

document.addEventListener("keydown", e => {
  if (e.key.toLowerCase() === 'v') {
    debugMode = !debugMode;
    
    swarm.drones.forEach(drone => {
      if (debugMode) {
        // Mostrar área de colisión
        const debugGeometry = new THREE.RingGeometry(
          drone.radius - 0.5,
          drone.radius + 0.5,
          16
        );
        const debugMaterial = new THREE.MeshBasicMaterial({
          color: 0xff0000,
          transparent: true,
          opacity: 0.3
        });
        drone.debugRing = new THREE.Mesh(debugGeometry, debugMaterial);
        drone.debugRing.position.copy(drone.mesh.position);
        scene.add(drone.debugRing);
      } else if (drone.debugRing) {
        // Remover debug
        scene.remove(drone.debugRing);
        drone.debugRing.geometry.dispose();
        drone.debugRing.material.dispose();
        drone.debugRing = null;
      }
    });
  }
});