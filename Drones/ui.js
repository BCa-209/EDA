/* =========================
   MANEJADOR DE INTERFAZ (UI)
========================= */
import { params, figureConfigs } from './config.js';
import { loadFigureFromJSON } from './figureProcessor.js';
import { canvasToPoints, textToPoints } from './utils.js';

export function setupUI(swarm, recreateSwarmCallback) {
    const inputCanvas = document.getElementById("inputCanvas");
    
    // Función para redimensionar canvas de dibujo
    const resizeCanvas = () => {
        inputCanvas.width = Math.min(800, window.innerWidth - 40);
        inputCanvas.height = 200;
    };
    window.addEventListener('resize', resizeCanvas);
    resizeCanvas();

    // --- Controles de Dibujo ---
    const ctx = inputCanvas.getContext("2d");
    let drawing = false;

    inputCanvas.addEventListener("mousedown", () => drawing = true);
    window.addEventListener("mouseup", () => drawing = false);
    
    inputCanvas.addEventListener("mousemove", e => {
        if (!drawing) return;
        const rect = inputCanvas.getBoundingClientRect();
        const x = e.clientX - rect.left;
        const y = e.clientY - rect.top;
        
        ctx.fillStyle = "white";
        ctx.beginPath();
        ctx.arc(x, y, 5, 0, Math.PI * 2);
        ctx.fill();
    });

    document.getElementById("drawBtn").onclick = () => {
        const points = canvasToPoints(inputCanvas);
        params.currentFigure = null;
        swarm.setFormation(points, 0xffffff);
        updateStats(swarm);
    };

    document.getElementById("clearBtn").onclick = () => {
        ctx.clearRect(0, 0, inputCanvas.width, inputCanvas.height);
        swarm.setFormation([], 0xffffff);
        updateStats(swarm);
    };

    // --- Texto ---
    document.getElementById("textBtn").onclick = () => {
        const txt = document.getElementById("textInput").value;
        const points = textToPoints(txt, inputCanvas);
        params.currentFigure = null;
        swarm.setFormation(points, 0xffffff);
        updateStats(swarm);
    };

    // --- Figuras JSON ---
    document.querySelectorAll('#figureButtons button').forEach(btn => {
        btn.onclick = async () => {
            const name = btn.dataset.figure;
            const data = await loadFigureFromJSON(name, params.droneCount);
            params.currentFigure = name;
            swarm.setFormation(data.points, data.color);
            updateStats(swarm);
            
            // UI Feedback
            document.querySelectorAll('#figureButtons button').forEach(b => b.classList.remove('active'));
            btn.classList.add('active');
        };
    });

    // --- Sliders y Parámetros ---
    const bindSlider = (id, paramKey) => {
        const el = document.getElementById(id);
        const valueId = id + 'Value'; // Asegurar que el ID del span sea correcto
        
        // Inicializar valor mostrado
        const label = document.getElementById(valueId);
        if(label) label.textContent = params[paramKey].toFixed(3);
        
        el.oninput = (e) => {
            params[paramKey] = parseFloat(e.target.value);
            // Actualizar label
            if(label) label.textContent = params[paramKey].toFixed(3);
        };
    };

    bindSlider("speedRange", "droneSpeed");
    bindSlider("orbitRadius", "baseRadius");
    bindSlider("orbitSpeed", "baseOrbitSpeed");
    bindSlider("personalSpaceRange", "personalSpace");
    bindSlider("separationForceRange", "separationForce");

    document.getElementById("collisionsToggle").onchange = (e) => {
        params.collisionsEnabled = e.target.checked;
        document.getElementById("collisionsStatus").textContent = params.collisionsEnabled ? "ON" : "OFF";
    };

    // --- Gestión de Cantidad de Drones ---
    document.getElementById("applyDrones").onclick = () => {
        const count = parseInt(document.getElementById("droneCount").value);
        if(count > 0 && count <= 5000) {
            params.droneCount = count;
            recreateSwarmCallback(count); // Llamada al main para reiniciar
        }
    };

    // --- Botones Extra ---
    document.getElementById("randomBtn").onclick = () => {
        swarm.setFormation([], 0xffffff);
        updateStats(swarm);
    };

    document.getElementById("centerBtn").onclick = () => {
        swarm.drones.forEach(d => { d.target.set(0,0,0); d.isOrbiting = false; });
    };

    document.getElementById("scaleUpBtn").onclick = () => {
        if (params.currentFigure && swarm.currentPoints) {
            // Escalar todos los puntos un 20% más
            const scaledPoints = swarm.currentPoints.map(p => ({
                x: p.x * 1.2,
                y: p.y * 1.2
            }));
            swarm.setFormation(scaledPoints, swarm.drones[0]?.originalColor || 0xffffff);
            updateStats(swarm);
        } else {
            // Si no hay figura, escalar todos los drones aleatoriamente
            swarm.drones.forEach(d => {
                d.target.x *= 1.2;
                d.target.y *= 1.2;
            });
        }
    };

    document.getElementById("scaleDownBtn").onclick = () => {
        if (params.currentFigure && swarm.currentPoints) {
            // Escalar todos los puntos un 20% menos
            const scaledPoints = swarm.currentPoints.map(p => ({
                x: p.x * 0.8,
                y: p.y * 0.8
            }));
            swarm.setFormation(scaledPoints, swarm.drones[0]?.originalColor || 0xffffff);
            updateStats(swarm);
        } else {
            // Si no hay figura, escalar todos los drones aleatoriamente
            swarm.drones.forEach(d => {
                d.target.x *= 0.8;
                d.target.y *= 0.8;
            });
        }
    };

    document.getElementById("resetScaleBtn").onclick = async () => {
        if (params.currentFigure) {
            // Recargar la figura original
            const data = await loadFigureFromJSON(params.currentFigure, params.droneCount);
            swarm.setFormation(data.points, data.color);
            updateStats(swarm);
        } else {
            // Si no hay figura, volver a posición aleatoria
            swarm.setFormation([], 0xffffff);
            updateStats(swarm);
        }
    };

    // --- Atajos de Teclado ---
    document.addEventListener('keydown', (e) => {
        // Evitar atajos cuando se está escribiendo en inputs/textarea
        if (e.target.tagName === 'INPUT' || e.target.tagName === 'TEXTAREA') return;
        
        switch(e.key.toLowerCase()) {
            case '1':
                document.querySelector('[data-figure="mascara"]').click();
                break;
            case '2':
                document.querySelector('[data-figure="logo_escuela"]').click();
                break;
            case '3':
                document.querySelector('[data-figure="logo_universidad"]').click();
                break;
            case '4':
                document.querySelector('[data-figure="estrella"]').click();
                break;
            case ' ':
                document.getElementById("clearBtn").click();
                e.preventDefault(); // Evitar scroll
                break;
            case 't':
                document.getElementById("textBtn").click();
                break;
            case 'd':
                document.getElementById("drawBtn").click();
                break;
            case 'r':
                document.getElementById("randomBtn").click();
                break;
            case 'c':
                document.getElementById("centerBtn").click();
                break;
            case 'x':
                document.getElementById("collisionsToggle").click();
                break;
            case '+':
                if (e.ctrlKey) {
                    document.getElementById("scaleUpBtn").click();
                    e.preventDefault();
                }
                break;
            case '-':
                if (e.ctrlKey) {
                    document.getElementById("scaleDownBtn").click();
                    e.preventDefault();
                }
                break;
            case 'f5':
                document.getElementById("resetScaleBtn").click();
                e.preventDefault(); // Evitar recargar página
                break;
        }
    });

    // --- Loop de actualización de stats ---
    setInterval(() => updateStats(swarm), 500);
}

function updateStats(swarm) {
    const stats = swarm.getStats();
    document.getElementById("droneCountValue").textContent = stats.total;
    document.getElementById("formationCountValue").textContent = stats.formation || 0;
    
    // Actualizar según el modo activo
    if (stats.mode === 'Orbital') {
        document.getElementById("orbitalCountValue").textContent = stats.orbiting || 0;
        document.getElementById("orbitalCountValue").parentElement.style.display = 'flex';
    } else if (stats.mode === 'Eficiente') {
        document.getElementById("orbitalCountValue").textContent = stats.idle || 0;
        document.getElementById("orbitalCountValue").previousElementSibling.textContent = 'Inactivos:';
        document.getElementById("orbitalCountValue").parentElement.style.display = 'flex';
    } else if (stats.mode === 'Híbrido') {
        document.getElementById("orbitalCountValue").textContent = stats.animating || 0;
        document.getElementById("orbitalCountValue").previousElementSibling.textContent = 'Animando:';
        document.getElementById("orbitalCountValue").parentElement.style.display = 'flex';
    }
    
    if (params.currentFigure) {
        document.getElementById("currentFigure").textContent = figureConfigs[params.currentFigure]?.name || params.currentFigure;
        const color = figureConfigs[params.currentFigure]?.color || 0xffffff;
        document.getElementById("currentColor").style.backgroundColor = '#' + color.toString(16).padStart(6, '0');
    } else {
        document.getElementById("currentFigure").textContent = "Manual";
        document.getElementById("currentColor").style.backgroundColor = "white";
    }
}