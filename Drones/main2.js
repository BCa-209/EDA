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
let currentFigure = null;

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
   SISTEMA DE CARGA DE FIGURAS DESDE JSON - CORREGIDO
========================= */
const figureCache = new Map();

// Configuración de figuras desde JSON
const figureConfigs = {
  'mascara': { 
    jsonFile: 'figures/mascara.json', 
    scale: 3.0,  // Escala aumentada
    spacing: 3.0, // Más espacio entre puntos
    color: 0xFF5733,
    name: 'Máscara',
    icon: 'fa-mask',
    method: 'intelligent' // 'exact', 'borders', o 'intelligent'
  },
  'logo_escuela': { 
    jsonFile: 'figures/logo_escuela.json', 
    scale: 3.0,
    spacing: 3.0,
    color: 0x33A1FF,
    name: 'Logo Escuela',
    icon: 'fa-graduation-cap',
    method: 'intelligent'
  },
  'logo_universidad': {   
    jsonFile: 'figures/logo_universidad.json', 
    scale: 3.0,
    spacing: 3.0,
    color: 0x9D33FF,
    name: 'Logo Universidad',
    icon: 'fa-university',
    method: 'intelligent'
  },
  'estrella': { 
    jsonFile: 'figures/estrella.json', 
    scale: 3.0,
    spacing: 3.0,
    color: 0xFFFF33,
    name: 'Estrella',
    icon: 'fa-star',
    method: 'intelligent'
  }
};

/* =========================
   FUNCIONES DE PROCESAMIENTO DE MATRICES 128x128
========================= */

// 1. Procesamiento EXACTO - 1:1 mapping
function processMatrix128Exact(matrixRows, scale = 2.0, spacing = 2.0) {
  const points = [];
  const rows = matrixRows.length;
  const cols = matrixRows[0]?.length || 0;
  
  if (rows === 0 || cols === 0) return [];
  
  const centerX = cols / 2;
  const centerY = rows / 2;
  const grid = new Map();
  const gridSize = spacing;
  
  // Recorrer TODOS los píxeles
  for (let y = 0; y < rows; y++) {
    const row = matrixRows[y];
    for (let x = 0; x < cols; x++) {
      if (row[x] === '1') {
        const screenX = (x - centerX) * scale;
        const screenY = (centerY - y) * scale;
        
        const gridX = Math.floor(screenX / gridSize);
        const gridY = Math.floor(screenY / gridSize);
        const gridKey = `${gridX},${gridY}`;
        
        if (!grid.has(gridKey)) {
          points.push({ x: screenX, y: screenY });
          grid.set(gridKey, true);
          
          // Marcar celdas adyacentes para evitar superposición
          for (let dx = -1; dx <= 1; dx++) {
            for (let dy = -1; dy <= 1; dy++) {
              grid.set(`${gridX + dx},${gridY + dy}`, true);
            }
          }
        }
      }
    }
  }
  
  console.log(`Procesado EXACTO: ${points.length} puntos`);
  return points;
}

// 2. Procesamiento solo BORDES - mejor visual
function processMatrix128Borders(matrixRows, scale = 2.0, spacing = 2.0) {
  const points = [];
  const rows = matrixRows.length;
  const cols = matrixRows[0]?.length || 0;
  
  if (rows === 0 || cols === 0) return [];
  
  const centerX = cols / 2;
  const centerY = rows / 2;
  const grid = new Map();
  const gridSize = spacing;
  
  for (let y = 0; y < rows; y++) {
    const row = matrixRows[y];
    for (let x = 0; x < cols; x++) {
      if (row[x] === '1') {
        // Verificar si es borde
        let isBorder = false;
        
        // Coordenadas de vecinos
        const neighbors = [
          [x-1, y], [x+1, y], [x, y-1], [x, y+1],
          [x-1, y-1], [x+1, y-1], [x-1, y+1], [x+1, y+1]
        ];
        
        for (const [nx, ny] of neighbors) {
          if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
            if (matrixRows[ny][nx] === '0') {
              isBorder = true;
              break;
            }
          } else {
            isBorder = true; // Borde de la matriz
            break;
          }
        }
        
        if (isBorder) {
          const screenX = (x - centerX) * scale;
          const screenY = (centerY - y) * scale;
          
          const gridX = Math.floor(screenX / gridSize);
          const gridY = Math.floor(screenY / gridSize);
          const gridKey = `${gridX},${gridY}`;
          
          if (!grid.has(gridKey)) {
            points.push({ x: screenX, y: screenY });
            grid.set(gridKey, true);
            
            for (let dx = -1; dx <= 1; dx++) {
              for (let dy = -1; dy <= 1; dy++) {
                grid.set(`${gridX + dx},${gridY + dy}`, true);
              }
            }
          }
        }
      }
    }
  }
  
  console.log(`Procesado BORDES: ${points.length} puntos`);
  return points;
}

// 3. Procesamiento INTELIGENTE - muestreo adaptativo
function processMatrix128Intelligent(matrixRows, targetPoints = 1000, scale = 2.0, spacing = 2.0) {
  const points = [];
  const rows = matrixRows.length;
  const cols = matrixRows[0]?.length || 0;
  
  if (rows === 0 || cols === 0) return [];
  
  const centerX = cols / 2;
  const centerY = rows / 2;
  
  // Contar total de '1's y guardar posiciones
  let totalOnes = 0;
  const onesPositions = [];
  
  for (let y = 0; y < rows; y++) {
    const row = matrixRows[y];
    for (let x = 0; x < cols; x++) {
      if (row[x] === '1') {
        totalOnes++;
        onesPositions.push({ x, y });
      }
    }
  }
  
  // Calcular factor de muestreo
  let sampleFactor = 1;
  if (totalOnes > targetPoints) {
    sampleFactor = Math.max(1, Math.floor(totalOnes / targetPoints));
  }
  
  // Sistema de grid para espaciado
  const grid = new Map();
  const gridSize = spacing;
  
  // Muestrear inteligentemente
  for (let i = 0; i < onesPositions.length; i += sampleFactor) {
    const { x, y } = onesPositions[i];
    
    const screenX = (x - centerX) * scale;
    const screenY = (centerY - y) * scale;
    
    const gridX = Math.floor(screenX / gridSize);
    const gridY = Math.floor(screenY / gridSize);
    const gridKey = `${gridX},${gridY}`;
    
    if (!grid.has(gridKey)) {
      points.push({ x: screenX, y: screenY });
      
      // Marcar celdas adyacentes
      for (let dx = -1; dx <= 1; dx++) {
        for (let dy = -1; dy <= 1; dy++) {
          grid.set(`${gridX + dx},${gridY + dy}`, true);
        }
      }
    }
  }
  
  console.log(`Procesado INTELIGENTE: ${totalOnes} unos → ${points.length} puntos (factor: ${sampleFactor})`);
  return points;
}

/* =========================
   FUNCIÓN PRINCIPAL DE CARGA DE FIGURAS
========================= */
async function loadFigureFromJSON(figureName) {
  const config = figureConfigs[figureName];
  if (!config) {
    console.error(`Configuración no encontrada para figura: ${figureName}`);
    return getDefaultFigure(figureName);
  }

  const cacheKey = `${figureName}_${droneCount}`;
  
  if (figureCache.has(cacheKey)) {
    return figureCache.get(cacheKey);
  }

  try {
    console.log(`Cargando figura ${figureName} desde ${config.jsonFile}...`);
    
    const response = await fetch(config.jsonFile);
    if (!response.ok) {
      throw new Error(`HTTP ${response.status}: ${response.statusText}`);
    }
    
    const data = await response.json();
    
    if (!data.data || !Array.isArray(data.data)) {
      throw new Error("Formato JSON inválido: falta propiedad 'data'");
    }

    // Validar tamaño
    if (data.height !== 128 || data.width !== 128) {
      console.warn(`${config.name}: Matriz ${data.height}x${data.width}, esperaba 128x128`);
    }

    let points;
    const method = config.method || 'intelligent';
    const targetPoints = Math.min(droneCount * 0.8, 2000);
    
    // Elegir método de procesamiento
    switch(method) {
      case 'exact':
        points = processMatrix128Exact(data.data, config.scale, config.spacing);
        break;
      case 'borders':
        points = processMatrix128Borders(data.data, config.scale, config.spacing);
        break;
      case 'intelligent':
      default:
        points = processMatrix128Intelligent(data.data, targetPoints, config.scale, config.spacing);
        break;
    }
    
    // Calcular bounding box para diagnóstico
    let bounds = { minX: Infinity, maxX: -Infinity, minY: Infinity, maxY: -Infinity };
    points.forEach(p => {
      bounds.minX = Math.min(bounds.minX, p.x);
      bounds.maxX = Math.max(bounds.maxX, p.x);
      bounds.minY = Math.min(bounds.minY, p.y);
      bounds.maxY = Math.max(bounds.maxY, p.y);
    });
    
    const width = bounds.maxX - bounds.minX;
    const height = bounds.maxY - bounds.minY;
    
    console.log(`${config.name}: ${points.length} puntos, tamaño: ${width.toFixed(1)}x${height.toFixed(1)}`);
    
    // Si la figura es muy pequeña, escalar automáticamente
    let finalScale = config.scale;
    let finalPoints = points;
    
    if (width < 100 && height < 100 && points.length > 0) {
      finalScale = config.scale * 2.5;
      finalPoints = points.map(p => ({
        x: p.x * 2.5,
        y: p.y * 2.5
      }));
      console.log(`Figura pequeña detectada, escalando automáticamente a ${finalScale}x`);
    }
    
    const figureData = {
      points: finalPoints,
      color: config.color,
      scale: finalScale,
      pointCount: finalPoints.length,
      name: config.name,
      icon: config.icon,
      bounds: bounds,
      originalScale: config.scale,
      method: method
    };

    figureCache.set(cacheKey, figureData);
    
    return figureData;

  } catch (error) {
    console.error(`Error cargando ${figureName}:`, error);
    return getDefaultFigure(figureName);
  }
}

/* =========================
   FIGURAS POR DEFECTO (fallback)
========================= */
function getDefaultFigure(figureName) {
  const points = [];
  const config = figureConfigs[figureName];
  
  switch(figureName) {
    case 'mascara':
      for (let angle = 0; angle < Math.PI * 2; angle += 0.1) {
        const x = Math.cos(angle) * 100;
        const y = Math.sin(angle) * 80 + Math.sin(angle * 2) * 15;
        points.push({ x, y });
      }
      break;
      
    case 'logo_escuela':
      for (let i = -80; i <= 80; i += 15) {
        points.push({ x: i, y: -40 });
        points.push({ x: i, y: 40 });
      }
      for (let i = -40; i <= 40; i += 15) {
        points.push({ x: -80, y: i });
        points.push({ x: 80, y: i });
      }
      break;
      
    case 'logo_universidad':
      for (let i = 0; i < 36; i++) {
        const angle = (i * 10 * Math.PI) / 180;
        const radius = i % 2 === 0 ? 90 : 70;
        points.push({
          x: Math.cos(angle) * radius,
          y: Math.sin(angle) * radius
        });
      }
      break;
      
    case 'estrella':
      const spikes = 5;
      const outerRadius = 100;
      const innerRadius = 50;
      
      for (let i = 0; i < spikes * 2; i++) {
        const radius = i % 2 === 0 ? outerRadius : innerRadius;
        const angle = (i * Math.PI) / spikes - Math.PI / 2;
        points.push({
          x: Math.cos(angle) * radius,
          y: Math.sin(angle) * radius
        });
      }
      points.push(points[0]);
      break;
  }
  
  return {
    points: points,
    color: config?.color || 0xFFFFFF,
    scale: 3.0,
    pointCount: points.length,
    name: config?.name || figureName,
    icon: config?.icon || 'fa-shapes',
    bounds: { minX: -100, maxX: 100, minY: -100, maxY: 100 }
  };
}

/* =========================
   TEXTO / CANVAS → PUNTOS
========================= */
function canvasToPoints() {
  const img = inputCtx.getImageData(0, 0, inputCanvas.width, inputCanvas.height);
  
  let activePixels = 0;
  for (let i = 3; i < img.data.length; i += 4) {
    if (img.data[i] > 128) activePixels++;
  }
  
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
  
  while (fontSize > 20) {
    inputCtx.font = `bold ${fontSize}px Arial`;
    
    const widestLine = Math.max(
      ...lines.map(l => inputCtx.measureText(l).width)
    );
    
    const totalHeight = lines.length * fontSize * lineSpacing;
    
    if (widestLine <= maxWidth && totalHeight <= maxHeight) break;
    fontSize--;
  }
  
  const startY = inputCanvas.height / 2 - ((lines.length - 1) * fontSize * lineSpacing) / 2;
  
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

inputCanvas.addEventListener("mousedown", e => {
  drawing = true;
  const r = inputCanvas.getBoundingClientRect();
  lastX = e.clientX - r.left;
  lastY = e.clientY - r.top;
  
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
  constructor(x, y, color = 0xffffff) {
    const g = new THREE.CircleGeometry(2, 10);
    const m = new THREE.MeshBasicMaterial({ color: color });
    this.mesh = new THREE.Mesh(g, m);
    this.mesh.position.set(x, y, 0);
    this.target = new THREE.Vector3(x, y, 0);
    this.isOrbiting = false;
    this.originalColor = color;
    this.orbitColor = 0x00ffff;
    this.colorTransition = 0;
    this.pulsePhase = Math.random() * Math.PI * 2;
    
    // Sistema de colisiones
    this.velocity = new THREE.Vector3(
      (Math.random() - 0.5) * 0.2,
      (Math.random() - 0.5) * 0.2,
      0
    );
    this.radius = 3;
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
        const overlap = minDistance - distance;
        const pushStrength = (overlap / minDistance) * this.pushForce;
        
        totalPushX += (dx / distance) * pushStrength;
        totalPushY += (dy / distance) * pushStrength;
        collisionCount++;
      }
    }
    
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
      avgVelocityX /= flockCount;
      avgVelocityY /= flockCount;
      const alignX = (avgVelocityX - this.velocity.x) * this.alignmentFactor;
      const alignY = (avgVelocityY - this.velocity.y) * this.alignmentFactor;
      
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
    
    if (distance > 0.5) {
      const targetSpeed = Math.min(this.maxSpeed, distance * 0.05);
      const moveX = (dx / distance) * targetSpeed;
      const moveY = (dy / distance) * targetSpeed;
      
      this.velocity.x += moveX * droneSpeed * 10;
      this.velocity.y += moveY * droneSpeed * 10;
    }
    
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
    this.originalColors = [];
    
    for (let i = 0; i < count; i++) {
      const x = Math.random() * width - width / 2;
      const y = Math.random() * height - height / 2;
      const d = new Drone(x, y);
      this.drones.push(d);
      this.originalColors.push(0xffffff);
      scene.add(d.mesh);
    }
    
    this.orbitDrones = [];
    this.orbitCenter = new THREE.Vector3(0, 0, 0);
    this.orbitTime = 0;
    this.currentPoints = [];
    this.orbitalCount = 0;
    this.currentFigureColor = 0xffffff;
    
    // Optimización para búsqueda de vecinos
    this.grid = new Map();
    this.cellSize = globalPersonalSpace * 3;
    
    this.updateInfo();
  }
  
  dispose() {
    this.drones.forEach(d => scene.remove(d.mesh));
    this.drones = [];
    this.orbitDrones = [];
    this.originalColors = [];
  }
  
  // Actualizar colores de los drones
  updateDroneColors(color) {
    this.currentFigureColor = color;
    this.drones.forEach((drone, i) => {
      drone.originalColor = color;
      this.originalColors[i] = color;
    });
  }
  
  // Restaurar colores originales (blanco)
  resetDroneColors() {
    this.currentFigureColor = 0xffffff;
    this.drones.forEach((drone, i) => {
      drone.originalColor = this.originalColors[i] || 0xffffff;
    });
  }
  
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
  
  getNeighbors(drone) {
    const neighbors = [];
    const cellX = Math.floor(drone.mesh.position.x / this.cellSize);
    const cellY = Math.floor(drone.mesh.position.y / this.cellSize);
    const searchRadius = Math.ceil(drone.personalSpace * 2 / this.cellSize);
    
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
  
  setFormation(points, figureColor = 0xffffff, centerAtOrigin = true) {
    let centeredPoints = [...points];
    
    // Centrar automáticamente en el origen si se solicita
    if (centerAtOrigin && points.length > 0) {
      let sumX = 0, sumY = 0;
      points.forEach(p => {
        sumX += p.x;
        sumY += p.y;
      });
      
      const centerX = sumX / points.length;
      const centerY = sumY / points.length;
      
      centeredPoints = points.map(p => ({
        x: p.x - centerX,
        y: p.y - centerY
      }));
    }
    
    this.currentPoints = centeredPoints;
    this.updateDroneColors(figureColor);
    
    // Resetear todos los drones
    this.drones.forEach(d => {
      d.isOrbiting = false;
      d.velocity.set(0, 0, 0);
    });
    
    this.orbitDrones = [];
    
    if (centeredPoints.length === 0) {
      // Si no hay puntos, todos van a órbita
      this.drones.forEach((drone, i) => {
        drone.isOrbiting = true;
        this.orbitDrones.push(i);
      });
    } else {
      // Distribuir drones uniformemente
      const dronesPerPoint = Math.max(1, Math.floor(this.drones.length / centeredPoints.length));
      
      for (let i = 0; i < this.drones.length; i++) {
        const drone = this.drones[i];
        
        if (i < centeredPoints.length * dronesPerPoint) {
          const pointIndex = Math.floor(i / dronesPerPoint) % centeredPoints.length;
          const p = centeredPoints[pointIndex];
          drone.target.set(p.x, p.y, 0);
          drone.isOrbiting = false;
        } else {
          drone.isOrbiting = true;
          this.orbitDrones.push(i);
        }
      }
    }
    
    // Calcular centro para órbita
    if (centeredPoints.length > 0) {
      const sum = centeredPoints.reduce((acc, p) => {
        acc.x += p.x;
        acc.y += p.y;
        return acc;
      }, { x: 0, y: 0 });
      
      this.orbitCenter.set(
        sum.x / centeredPoints.length,
        sum.y / centeredPoints.length,
        0
      );
    } else {
      this.orbitCenter.set(0, 0, 0);
    }
    
    this.orbitalCount = this.orbitDrones.length;
    this.updateInfo();
  }
  
  update() {
    this.orbitTime += 0.05;
    this.updateGrid();
    
    // Calcular posiciones objetivo primero
    for (let i = 0; i < this.drones.length; i++) {
      const d = this.drones[i];
      
      if (d.isOrbiting) {
        const orbitIndex = this.orbitDrones.indexOf(i);
        
        const radiusVariation = (orbitIndex % 8) * 25;
        const orbitRadius = baseRadius + radiusVariation;
        
        const speedVariation = (orbitIndex % 13) * 0.003;
        const orbitSpeed = baseOrbitSpeed + speedVariation;
        
        const angle = this.orbitTime * orbitSpeed + orbitIndex * 0.15;
        
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
    const activeDrones = this.currentPoints.length > 0 
      ? Math.min(this.drones.length, this.currentPoints.length)
      : 0;
    
    document.getElementById("droneCountValue").textContent = this.drones.length;
    document.getElementById("orbitalCountValue").textContent = this.orbitalCount;
    document.getElementById("formationCountValue").textContent = activeDrones;
    document.getElementById("collisionsStatus").textContent = collisionsEnabled ? "ON" : "OFF";
    
    // Actualizar información de figura actual
    if (currentFigure) {
      const config = figureConfigs[currentFigure];
      if (config) {
        document.getElementById("currentFigure").textContent = config.name;
        const colorElement = document.getElementById("currentColor");
        if (colorElement) {
          const colorHex = this.currentFigureColor.toString(16).padStart(6, '0');
          colorElement.style.backgroundColor = `#${colorHex}`;
        }
      }
    } else {
      document.getElementById("currentFigure").textContent = "Personalizado";
      const colorElement = document.getElementById("currentColor");
      if (colorElement) {
        colorElement.style.backgroundColor = "#ffffff";
      }
    }
  }
  
  updateCollisionParams() {
    this.drones.forEach(d => {
      d.personalSpace = globalPersonalSpace;
      d.maxForce = globalSeparationForce;
      d.pushForce = globalSeparationForce * 0.6;
      this.cellSize = globalPersonalSpace * 3;
    });
    this.updateInfo();
  }
}

/* =========================
   UI CONTROLS
========================= */
function updateSliderValues() {
  document.getElementById("speedValue").textContent = droneSpeed.toFixed(2);
  document.getElementById("radiusValue").textContent = baseRadius;
  document.getElementById("orbitSpeedValue").textContent = baseOrbitSpeed.toFixed(3);
  document.getElementById("personalSpaceValue").textContent = globalPersonalSpace;
  document.getElementById("separationForceValue").textContent = globalSeparationForce.toFixed(1);
}

// Función para aplicar figura
async function applyFigure(figureName) {
  currentFigure = figureName;
  
  try {
    const figureData = await loadFigureFromJSON(figureName);
    
    swarm.setFormation(figureData.points, figureData.color, true);
    
    // Actualizar estado activo de botones
    document.querySelectorAll('#figureButtons button').forEach(btn => {
      btn.classList.remove('active');
      if (btn.dataset.figure === figureName) {
        btn.classList.add('active');
      }
    });
    
    clearInputCanvas();
    
    console.log(`${figureData.name} aplicada: ${figureData.pointCount} puntos, escala: ${figureData.scale}x`);
    
  } catch (error) {
    console.error(`Error aplicando figura ${figureName}:`, error);
  }
}

// Configurar botones de figuras
document.querySelectorAll('#figureButtons button').forEach(btn => {
  btn.addEventListener('click', () => {
    const figureName = btn.dataset.figure;
    if (figureName && figureConfigs[figureName]) {
      applyFigure(figureName);
    }
  });
});

// Texto
document.getElementById("textBtn").onclick = () => {
  const t = document.getElementById("textInput").value;
  if (t.trim()) {
    currentFigure = null;
    swarm.resetDroneColors();
    document.querySelectorAll('#figureButtons button').forEach(btn => {
      btn.classList.remove('active');
    });
    swarm.setFormation(textToPoints(t), 0xffffff, true);
  }
};

// Dibujo
document.getElementById("drawBtn").onclick = () => {
  const p = canvasToPoints();
  if (p.length > 0) {
    currentFigure = null;
    swarm.resetDroneColors();
    document.querySelectorAll('#figureButtons button').forEach(btn => {
      btn.classList.remove('active');
    });
    swarm.setFormation(p, 0xffffff, true);
  } else {
    const t = document.getElementById("textInput").value || "HOLA";
    currentFigure = null;
    swarm.resetDroneColors();
    swarm.setFormation(textToPoints(t), 0xffffff, true);
  }
};

// Limpiar
document.getElementById("clearBtn").onclick = () => {
  clearInputCanvas();
  currentFigure = null;
  swarm.resetDroneColors();
  document.querySelectorAll('#figureButtons button').forEach(btn => {
    btn.classList.remove('active');
  });
  swarm.setFormation([], 0xffffff, true);
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
  swarm.updateInfo();
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
    const oldColor = swarm.currentFigureColor;
    swarm.dispose();
    swarm = new Swarm(droneCount);
    
    // Restaurar formación si existía
    if (oldPoints && oldPoints.length > 0) {
      setTimeout(() => swarm.setFormation(oldPoints, oldColor, false), 100);
    }
    
    swarm.updateCollisionParams();
  }
};

// Botón aleatorio
document.getElementById("randomBtn").onclick = () => {
  swarm.resetDroneColors();
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
  currentFigure = null;
  swarm.updateInfo();
};

// Botón centrar
document.getElementById("centerBtn").onclick = () => {
  swarm.resetDroneColors();
  swarm.drones.forEach(d => {
    d.target.set(0, 0, 0);
    d.isOrbiting = false;
    d.velocity.set(0, 0, 0);
  });
  swarm.orbitDrones = [];
  swarm.orbitalCount = 0;
  swarm.orbitCenter.set(0, 0, 0);
  currentFigure = null;
  swarm.updateInfo();
};

// Botón para escalar figura
document.getElementById("scaleUpBtn").onclick = () => {
  if (currentFigure && swarm.currentPoints.length > 0) {
    const scaledPoints = swarm.currentPoints.map(p => ({
      x: p.x * 1.2,
      y: p.y * 1.2
    }));
    swarm.setFormation(scaledPoints, swarm.currentFigureColor, false);
  }
};

document.getElementById("scaleDownBtn").onclick = () => {
  if (currentFigure && swarm.currentPoints.length > 0) {
    const scaledPoints = swarm.currentPoints.map(p => ({
      x: p.x * 0.8,
      y: p.y * 0.8
    }));
    swarm.setFormation(scaledPoints, swarm.currentFigureColor, false);
  }
};

document.getElementById("resetScaleBtn").onclick = async () => {
  if (currentFigure) {
    figureCache.delete(`${currentFigure}_${droneCount}`);
    await applyFigure(currentFigure);
  }
};

/* =========================
   INIT
========================= */
swarm = new Swarm(droneCount);
swarm.updateCollisionParams();

// Precargar figuras en segundo plano
setTimeout(() => {
  Object.keys(figureConfigs).forEach(figureName => {
    loadFigureFromJSON(figureName).then(figureData => {
      console.log(`Precargada: ${figureData.name} (${figureData.pointCount} puntos)`);
    }).catch(() => {
      console.log(`Figura ${figureName} no disponible`);
    });
  });
}, 1000);

// Formar texto inicial
setTimeout(() => {
  const initialText = document.getElementById("textInput").value;
  if (initialText.trim()) {
    swarm.setFormation(textToPoints(initialText), 0xffffff, true);
  }
}, 500);

updateSliderValues();

/* =========================
   ANIMATION LOOP
========================= */
let lastTime = 0;
const fixedTimeStep = 1000 / 60;

function animate(currentTime) {
  requestAnimationFrame(animate);
  
  const deltaTime = currentTime - lastTime || 0;
  lastTime = currentTime;
  
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
  resizeInputCanvas();
  
  if (swarm.currentPoints.length > 0 && !currentFigure) {
    const textarea = document.getElementById("textInput");
    if (textarea.value.trim()) {
      clearInputCanvas();
      const points = textToPoints(textarea.value);
      swarm.setFormation(points, 0xffffff, true);
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
      currentFigure = null;
      swarm.resetDroneColors();
      document.querySelectorAll('#figureButtons button').forEach(btn => {
        btn.classList.remove('active');
      });
      swarm.setFormation([], 0xffffff, true);
      break;
    case "1":
      applyFigure("mascara");
      break;
    case "2":
      applyFigure("logo_escuela");
      break;
    case "3":
      applyFigure("logo_universidad");
      break;
    case "4":
      applyFigure("estrella");
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
      if (e.ctrlKey || e.metaKey) {
        e.preventDefault();
        document.getElementById("scaleUpBtn").click();
      } else {
        baseRadius = Math.min(baseRadius + 10, 300);
        document.getElementById("orbitRadius").value = baseRadius;
        updateSliderValues();
      }
      break;
    case "-":
      if (e.ctrlKey || e.metaKey) {
        e.preventDefault();
        document.getElementById("scaleDownBtn").click();
      } else {
        baseRadius = Math.max(baseRadius - 10, 50);
        document.getElementById("orbitRadius").value = baseRadius;
        updateSliderValues();
      }
      break;
    case "x":
      collisionsEnabled = !collisionsEnabled;
      document.getElementById("collisionsToggle").checked = collisionsEnabled;
      swarm.updateInfo();
      break;
    case "f5":
    case "f":
      if (currentFigure) {
        document.getElementById("resetScaleBtn").click();
      }
      break;
  }
});