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
  1000,
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
let swarm;

let baseRadius = 120;
let baseSpeed = 0.015;

document.getElementById("orbitRadius").oninput = (e) => {
  baseRadius = parseFloat(e.target.value);
};

document.getElementById("orbitSpeed").oninput = (e) => {
  baseSpeed = parseFloat(e.target.value);
};
/* =========================
   CANVAS INPUT
========================= */
const inputCanvas = document.getElementById("inputCanvas");
const inputCtx = inputCanvas.getContext("2d");

function clearInputCanvas() {
  inputCtx.clearRect(0, 0, inputCanvas.width, inputCanvas.height);
}

/* =========================
   TEXTO / CANVAS → PUNTOS
========================= */
function canvasToPoints() {
  const img = inputCtx.getImageData(
    0,
    0,
    inputCanvas.width,
    inputCanvas.height
  );

  // Contar píxeles activos
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
          y: inputCanvas.height / 2 - y,
        });
      }
    }
  }

  return points;
}

function textToPoints(text) {
  clearInputCanvas();

  const lines = text.split("\n");

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
  const startY =
    inputCanvas.height / 2 -
    ((lines.length - 1) * fontSize * lineSpacing) / 2;

  lines.forEach((line, i) => {
  const y = startY + i * fontSize * lineSpacing;

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

inputCanvas.addEventListener("mousedown", (e) => {
  drawing = true;
  const r = inputCanvas.getBoundingClientRect();
  lastX = e.clientX - r.left;
  lastY = e.clientY - r.top;
});

["mouseup", "mouseleave"].forEach((ev) =>
  inputCanvas.addEventListener(ev, () => (drawing = false)),
);

inputCanvas.addEventListener("mousemove", (e) => {
  if (!drawing) return;

  const r = inputCanvas.getBoundingClientRect();
  const x = e.clientX - r.left;
  const y = e.clientY - r.top;

  inputCtx.strokeStyle = "white";
  inputCtx.lineWidth = 6;
  inputCtx.lineCap = "round";

  inputCtx.beginPath();
  inputCtx.moveTo(lastX, lastY);
  inputCtx.lineTo(x, y);
  inputCtx.stroke();

  lastX = x;
  lastY = y;
});

/* =========================
   DRONE
========================= */
class Drone {
  constructor(x, y) {
    const g = new THREE.CircleGeometry(2, 10);
    const m = new THREE.MeshBasicMaterial({ color: 0xffffff });
    this.mesh = new THREE.Mesh(g, m);
    this.mesh.position.set(x, y, 0);
    this.target = new THREE.Vector3(x, y, 0);
    this.isOrbiting = false; // Nuevo flag
  }

  update() {
    this.mesh.position.lerp(this.target, droneSpeed);

    // Opcional: Cambiar color si está en órbita
    if (this.isOrbiting) {
      this.mesh.material.color.setHex(0x00ffff); // Cian para drones orbitales
    } else {
      this.mesh.material.color.setHex(0xffffff); // Blanco para drones en formación
    }
  }
}

/* =========================
   SWARM
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
    this.orbitDrones = []; // Índices de drones en órbita
    this.orbitCenter = new THREE.Vector3(0, 0, 0);
    this.orbitTime = 0;
    this.currentPoints = []; // Para restaurar después de aplicar nuevos drones
  }

  dispose() {
    this.drones.forEach((d) => scene.remove(d.mesh));
  }

  setFormation(points) {
    // Guardar puntos actuales
    this.currentPoints = [...points];
    
    // Resetear todos los drones
    this.drones.forEach((d) => {
      d.isOrbiting = false;
    });

    // Distribuir drones uniformemente
    this.orbitDrones = [];
    
    // Calcular cuántos drones por punto
    const dronesPerPoint = Math.max(1, Math.floor(this.drones.length / Math.max(points.length, 1)));
    
    for (let i = 0; i < this.drones.length; i++) {
      const drone = this.drones[i];
      
      if (i < points.length * dronesPerPoint && points.length > 0) {
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

    // Calcular centro de la formación para órbita
    if (points.length > 0) {
      const sum = points.reduce(
        (acc, p) => {
          acc.x += p.x;
          acc.y += p.y;
          return acc;
        },
        { x: 0, y: 0 },
      );

      this.orbitCenter.set(sum.x / points.length, sum.y / points.length, 0);
    } else {
      // Si no hay puntos, centro en pantalla
      this.orbitCenter.set(0, 0, 0);
    }
  }

  update() {
    // Incrementar tiempo para animación orbital
    this.orbitTime += 0.05;

    // Actualizar drones
    for (let i = 0; i < this.drones.length; i++) {
      const d = this.drones[i];

      if (d.isOrbiting) {
  const orbitIndex = this.orbitDrones.indexOf(i);
  
  // Efecto de pulso en el radio
  const pulse = Math.sin(this.orbitTime * 0.3) * 0.15 + 1; // Entre 0.85 y 1.15
  
  // USAR LAS VARIABLES GLOBALES baseRadius y baseSpeed
  const radiusVariation = (orbitIndex % 8) * 25;
  const orbitRadius = (baseRadius + radiusVariation) * pulse; // Con pulso

  const speedVariation = (orbitIndex % 13) * 0.003;
  const orbitSpeed = baseSpeed + speedVariation;

  // Ángulo con desfase único por drone
  const angle = this.orbitTime * orbitSpeed + orbitIndex * 0.15;

  // Posición orbital con pequeña variación aleatoria
  const wobbleX = Math.sin(this.orbitTime * 0.2 + orbitIndex) * 8;
  const wobbleY = Math.cos(this.orbitTime * 0.25 + orbitIndex) * 8;
  
  d.target.x = this.orbitCenter.x + Math.cos(angle) * orbitRadius + wobbleX;
  d.target.y = this.orbitCenter.y + Math.sin(angle) * orbitRadius + wobbleY;
}

      d.update();
    }
  }
}

/* =========================
   UI CONTROLS
========================= */
document.getElementById("textBtn").onclick = () => {
  const t = document.getElementById("textInput").value;
  if (t) swarm.setFormation(textToPoints(t));
};

document.getElementById("drawBtn").onclick = () => {
  const p = canvasToPoints();
  if (p.length) swarm.setFormation(p);
};

document.getElementById("clearBtn").onclick = clearInputCanvas;

document.getElementById("speedRange").oninput = (e) => {
  droneSpeed = parseFloat(e.target.value);
};

document.getElementById("applyDrones").onclick = () => {
  droneCount = parseInt(document.getElementById("droneCount").value);
  const oldPoints = swarm.currentPoints; // Guardar formación actual
  swarm.dispose();
  swarm = new Swarm(droneCount);
  
  // Restaurar formación si existía
  if (oldPoints && oldPoints.length > 0) {
    setTimeout(() => swarm.setFormation(oldPoints), 100);
  }
};

/* =========================
   INIT
========================= */
swarm = new Swarm(droneCount);

/* =========================
   LOOP
========================= */
function animate() {
  requestAnimationFrame(animate);
  swarm.update();
  renderer.render(scene, camera);
}
animate();

/* =========================
   RESIZE
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
});
