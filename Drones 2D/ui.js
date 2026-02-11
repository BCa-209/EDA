/* =========================
   MANEJADOR DE INTERFAZ (UI)
========================= */
import { params, figureConfigs } from './config.js';
import { loadFigureFromJSON } from './figureProcessor.js';
import { canvasToPoints, textToPoints } from './utils.js';

export function setupUI(swarm, recreateSwarmCallback) {
    const inputCanvas = document.getElementById("inputCanvas");
    
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
        
        // CORREGIDO: Pasar null como nombre de figura
        swarm.setFormation(points, 0xffffff, null);
        updateStats(swarm);
    };

    document.getElementById("clearBtn").onclick = () => {
        ctx.clearRect(0, 0, inputCanvas.width, inputCanvas.height);
        params.currentFigure = null;
        
        // CORREGIDO: Limpiar formación
        swarm.setFormation([], 0xffffff, null);
        updateStats(swarm);
    };

    // --- Texto ---
    document.getElementById("textBtn").onclick = () => {
        const txt = document.getElementById("textInput").value;
        const points = textToPoints(txt, inputCanvas);
        params.currentFigure = null;
        
        // CORREGIDO: Pasar null como nombre de figura
        swarm.setFormation(points, 0xffffff, null);
        updateStats(swarm);
    };

    // --- Figuras JSON ---
    document.querySelectorAll('#figureButtons button').forEach(btn => {
        btn.onclick = async () => {
            const name = btn.dataset.figure;
            const data = await loadFigureFromJSON(name, params.droneCount);
            
            // CORREGIDO: Actualizar params y swarm correctamente
            params.currentFigure = name;
            
            // CORREGIDO: Pasar el nombre de la figura
            swarm.setFormation(data.points, data.color, name);
            updateStats(swarm);
            
            document.querySelectorAll('#figureButtons button').forEach(b => b.classList.remove('active'));
            btn.classList.add('active');
        };
    });

    // --- Sliders y Parámetros ---
    const bindSlider = (id, paramKey) => {
        const el = document.getElementById(id);
        const valueId = id + 'Value';
        
        const label = document.getElementById(valueId);
        if(label) label.textContent = params[paramKey].toFixed(3);
        
        el.oninput = (e) => {
            params[paramKey] = parseFloat(e.target.value);
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
    // --- Gestión de Cantidad de Drones ---
document.getElementById("applyDrones").onclick = async () => {
    const count = parseInt(document.getElementById("droneCount").value);
    if(count > 0 && count <= 5000) {
        params.droneCount = count;
        
        // Guardar TODA la información actual
        const currentFormation = {
            points: swarm.currentPoints || [],
            color: swarm.formationColor || 0xffffff,
            figureName: swarm.currentFigureName || params.currentFigure,
            mode: swarm.mode
        };
        
        console.log("=== ANTES DE RECREAR ENJAMBRE ===");
        console.log("Figura actual:", currentFormation.figureName);
        console.log("Puntos:", currentFormation.points.length);
        console.log("Color:", currentFormation.color.toString(16));
        console.log("Modo:", currentFormation.mode);
        
        // Si hay una figura activa, recargarla
        let figureData = null;
        if (currentFormation.figureName) {
            console.log("Recargando figura desde JSON...");
            figureData = await loadFigureFromJSON(currentFormation.figureName, count);
            console.log("Figura recargada con", figureData.points.length, "puntos");
        }
        
        // Recrear swarm y esperar a que esté listo
        await recreateSwarm(count);
        
        console.log("=== DESPUÉS DE RECREAR ENJAMBRE ===");
        console.log("Swarm nuevo figureName:", swarm.currentFigureName);
        console.log("Swarm activeMode disponible:", swarm.activeMode ? "Sí" : "No");
        
        // Solo restaurar si había una figura activa
        if (figureData && currentFormation.figureName) {
            console.log("Aplicando formación restaurada...");
            
            // Usar los datos recargados de la figura
            swarm.setFormation(
                figureData.points, 
                figureData.color,
                currentFormation.figureName
            );
            
            // Actualizar parámetros
            params.currentFigure = currentFormation.figureName;
            
            // Actualizar botón activo
            document.querySelectorAll('#figureButtons button').forEach(b => {
                b.classList.remove('active');
                if (b.dataset.figure === currentFormation.figureName) {
                    b.classList.add('active');
                }
            });
            
            console.log(`Figura ${currentFormation.figureName} restaurada con éxito`);
        } else if (currentFormation.points.length > 0) {
            // Si había puntos manuales (dibujo/texto)
            console.log("Restaurando puntos manuales...");
            swarm.setFormation(
                currentFormation.points,
                currentFormation.color,
                null
            );
        } else {
            console.log("No hay formación para restaurar");
        }
        
        // Actualizar estadísticas
        updateStats(swarm);
    }
};

    // --- Botones Extra ---
    document.getElementById("randomBtn").onclick = () => {
        params.currentFigure = null;
        swarm.setFormation([], 0xffffff, null);
        updateStats(swarm);
    };

    document.getElementById("centerBtn").onclick = () => {
        swarm.drones.forEach(d => { 
            d.target.set(0, 0, 0); 
            d.isOrbiting = false; 
        });
    };

    document.getElementById("scaleUpBtn").onclick = () => {
        if (swarm.activeMode && swarm.activeMode.currentPoints && swarm.activeMode.currentPoints.length > 0) {
            const currentPoints = swarm.activeMode.currentPoints;
            const scaledPoints = currentPoints.map(p => ({
                x: p.x * 1.2,
                y: p.y * 1.2
            }));
            const color = swarm.activeMode.currentColor || 0xffffff;
            const figureName = swarm.activeMode.currentFigureName || null;
            
            swarm.setFormation(scaledPoints, color, figureName);
            updateStats(swarm);
        } else {
            swarm.drones.forEach(d => {
                d.target.x *= 1.2;
                d.target.y *= 1.2;
            });
        }
    };

    document.getElementById("scaleDownBtn").onclick = () => {
        if (swarm.activeMode && swarm.activeMode.currentPoints && swarm.activeMode.currentPoints.length > 0) {
            const currentPoints = swarm.activeMode.currentPoints;
            const scaledPoints = currentPoints.map(p => ({
                x: p.x * 0.8,
                y: p.y * 0.8
            }));
            const color = swarm.activeMode.currentColor || 0xffffff;
            const figureName = swarm.activeMode.currentFigureName || null;
            
            swarm.setFormation(scaledPoints, color, figureName);
            updateStats(swarm);
        } else {
            swarm.drones.forEach(d => {
                d.target.x *= 0.8;
                d.target.y *= 0.8;
            });
        }
    };

    document.getElementById("resetScaleBtn").onclick = async () => {
        if (params.currentFigure) {
            const data = await loadFigureFromJSON(params.currentFigure, params.droneCount);
            swarm.setFormation(data.points, data.color, params.currentFigure);
            updateStats(swarm);
        } else {
            swarm.setFormation([], 0xffffff, null);
            updateStats(swarm);
        }
    };

    // --- Atajos de Teclado ---
    document.addEventListener('keydown', (e) => {
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
                e.preventDefault();
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
                e.preventDefault();
                break;
        }
    });

    setInterval(() => updateStats(swarm), 500);
}

function updateStats(swarm) {
    const stats = swarm.getStats();
    document.getElementById("droneCountValue").textContent = stats.total;
    
    if (stats.mode === 'Orbital') {
        document.getElementById("formationCountValue").textContent = stats.formation || 0;
        document.getElementById("orbitalCountValue").textContent = stats.orbiting || 0;
        document.getElementById("orbitalCountValue").previousElementSibling.textContent = 'Orbitales:';
    } else if (stats.mode === 'Eficiente') {
        document.getElementById("formationCountValue").textContent = stats.formation || 0;
        document.getElementById("orbitalCountValue").textContent = stats.idle || 0;
        document.getElementById("orbitalCountValue").previousElementSibling.textContent = 'Inactivos:';
    } else {
        document.getElementById("formationCountValue").textContent = stats.formation || 0;
        document.getElementById("orbitalCountValue").textContent = stats.orbiting || 0;
    }
    
    // CORREGIDO: Usar figura actual del swarm
    const currentFigureName = swarm.currentFigureName || params.currentFigure;
    if (currentFigureName) {
        document.getElementById("currentFigure").textContent = figureConfigs[currentFigureName]?.name || currentFigureName;
        const color = figureConfigs[currentFigureName]?.color || 0xffffff;
        document.getElementById("currentColor").style.backgroundColor = '#' + color.toString(16).padStart(6, '0');
    } else {
        document.getElementById("currentFigure").textContent = "Manual";
        document.getElementById("currentColor").style.backgroundColor = "white";
    }
    
    window.updateStats = updateStats;
}