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
let swarm;

let baseRadius = 120;
let baseSpeed = 0.015;

document.getElementById("orbitRadius").oninput = e =>
  (baseRadius = parseFloat(e.target.value));

document.getElementById("orbitSpeed").oninput = e =>
  (baseSpeed = parseFloat(e.target.value));

/* =========================
   CANVAS INPUT
========================= */
const inputCanvas = document.getElementById("inputCanvas");
const inputCtx = inputCanvas.getContext("2d");

function clearInputCanvas() {
  inputCtx.clearRect(0, 0, inputCanvas.width, inputCanvas.height);
}

/* =========================
   CANVAS → PUNTOS (DENSIDAD ADAPTATIVA)
========================= */
function canvasToPoints() {
  const img = inputCtx.getImageData(
    0,
    0,
    inputCanvas.width,
    inputCanvas.height
  );

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

/* =========================
   TEXTO MULTILÍNEA → PUNTOS
========================= */
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
  inputCtx.strokeStyle = "white";

  while (fontSize > 20) {
    inputCtx.font = `bold ${fontSize}px Arial`;

    const widest = Math.max(
      ...lines.map(l => inputCtx.measureText(l).width)
    );

    const totalHeight = lines.length * fontSize * lineSpacing;

    if (widest <= maxWidth && totalHeight <= maxHeight) break;
    fontSize--;
  }

  const startY =
    inputCanvas.height / 2 -
    ((lines.length - 1) * fontSize * lineSpacing) / 2;

  lines.forEach((line, i) => {
    const y = startY + i * fontSize * lineSpacing;

    inputCtx.lineWidth = 2;
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
});

["mouseup", "mouseleave"].forEach(ev =>
  inputCanvas.addEventListener(ev, () => (drawing = false))
);

inputCanvas.addEventListener("mousemove", e => {
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
    this.isOrbiting = false;
  }

  update() {
    this.mesh.position.lerp(this.target, droneSpeed);
    this.mesh.material.color.setHex(
      this.isOrbiting ? 0x00ffff : 0xffffff
    );
  }
}

/* =========================
   SWARM
========================= */
class Swarm {
  constructor(count) {
    this.drones = [];
    this.orbitDrones = [];
    this.orbitCenter = new THREE.Vector3();
    this.orbitTime = 0;

    for (let i = 0; i < count; i++) {
      const x = Math.random() * width - width / 2;
      const y = Math.random() * height - height / 2;
      const d = new Drone(x, y);
      this.drones.push(d);
      scene.add(d.mesh);
    }
  }

  dispose() {
    this.drones.forEach(d => scene.remove(d.mesh));
  }

  setFormation(points) {
    this.orbitDrones = [];

    this.drones.forEach((d, i) => {
      if (i < points.length) {
        const p = points[i];
        d.target.set(p.x, p.y, 0);
        d.isOrbiting = false;
      } else {
        d.isOrbiting = true;
        this.orbitDrones.push(i);
      }
    });

    if (points.length) {
      const c = points.reduce(
        (a, p) => ({ x: a.x + p.x, y: a.y + p.y }),
        { x: 0, y: 0 }
      );
      this.orbitCenter.set(c.x / points.length, c.y / points.length, 0);
    }
  }

  update() {
    this.orbitTime += 0.05;

    this.drones.forEach((d, i) => {
      if (d.isOrbiting) {
        const idx = this.orbitDrones.indexOf(i);
        const radius = baseRadius + (idx % 8) * 25;
        const speed = baseSpeed + (idx % 13) * 0.003;
        const angle = this.orbitTime * speed + idx * 0.15;

        d.target.x = this.orbitCenter.x + Math.cos(angle) * radius;
        d.target.y = this.orbitCenter.y + Math.sin(angle) * radius;
      }
      d.update();
    });
  }
}

/* =========================
   UI
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

document.getElementById("speedRange").oninput = e =>
  (droneSpeed = parseFloat(e.target.value));

document.getElementById("applyDrones").onclick = () => {
  droneCount = parseInt(document.getElementById("droneCount").value);
  swarm.dispose();
  swarm = new Swarm(droneCount);
};

/* =========================
   INIT & LOOP
========================= */
swarm = new Swarm(droneCount);

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
