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
  const img = inputCtx.getImageData(0, 0, inputCanvas.width, inputCanvas.height);
  const points = [];
  const step = 6;

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
  inputCtx.fillStyle = "white";
  inputCtx.font = "bold 120px Arial";
  inputCtx.textAlign = "center";
  inputCtx.textBaseline = "middle";
  inputCtx.fillText(text, inputCanvas.width / 2, inputCanvas.height / 2);
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
  }

  update() {
    this.mesh.position.lerp(this.target, droneSpeed);
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
  }

  dispose() {
    this.drones.forEach(d => scene.remove(d.mesh));
  }

  setFormation(points) {
    this.drones.forEach((d, i) => {
      const p = points[i % points.length];
      d.target.set(p.x, p.y, 0);
    });
  }

  update() {
    this.drones.forEach(d => d.update());
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

document.getElementById("speedRange").oninput = e => {
  droneSpeed = parseFloat(e.target.value);
};

document.getElementById("applyDrones").onclick = () => {
  droneCount = parseInt(document.getElementById("droneCount").value);
  swarm.dispose();
  swarm = new Swarm(droneCount);
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
