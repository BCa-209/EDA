/* =========================
   MAIN (PUNTO DE ENTRADA)
========================= */
import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.158.0/build/three.module.js';
import { SwarmSystem } from './swarmModes.js';
import { params } from './config.js';
import { setupUI } from './ui.js';

export function init() {
    const scene = new THREE.Scene();

    const camera = new THREE.OrthographicCamera(
        params.width / -2, params.width / 2,
        params.height / 2, params.height / -2, 
        0.1, 1000
    );
    camera.position.z = 10;

    const renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(params.width, params.height);
    renderer.setClearColor(0x000000);
    
    // Limpiar cualquier canvas anterior
    const oldCanvas = document.querySelector('canvas:not(#inputCanvas)');
    if (oldCanvas && oldCanvas !== renderer.domElement) {
        oldCanvas.remove();
    }
    
    document.body.appendChild(renderer.domElement);

    let swarm = new SwarmSystem(scene, params.droneCount);

const recreateSwarm = (newCount) => {
    console.log(`=== RECREANDO ENJAMBRE ===`);
    console.log(`Nuevo count: ${newCount}`);
    console.log(`Modo actual: ${swarm ? swarm.mode : 'none'}`);
    console.log(`Figura actual: ${swarm ? swarm.currentFigureName : 'none'}`);
    
    return new Promise((resolve) => {
        // Guardar información crítica ANTES de desechar
        const savedInfo = swarm ? {
            mode: swarm.mode,
            figureName: swarm.currentFigureName,
            formationColor: swarm.formationColor,
            currentPoints: swarm.currentPoints || []
        } : null;
        
        // Limpiar swarm actual
        if (swarm) {
            console.log("Descartando swarm anterior...");
            swarm.dispose();
            swarm = null;
        }
        
        // Crear nuevo swarm
        swarm = new SwarmSystem(scene, newCount);
        console.log("Nuevo swarm creado");
        
        // Restaurar modo si había uno
        if (savedInfo && savedInfo.mode) {
            console.log("Restaurando modo:", savedInfo.mode);
            swarm.setMode(savedInfo.mode);
        }
        
        // Esperar a que el swarm esté completamente inicializado
        setTimeout(() => {
            console.log("Swarm listo, activeMode:", swarm.activeMode ? "Sí" : "No");
            console.log("Swarm listo, figuraName actual:", swarm.currentFigureName);
            
            // Re-inicializar controles UI
            reinitializeUIControls(swarm);
            
            // Resolver la promesa con la información guardada
            resolve(savedInfo);
        }, 100);
    });
};

    setupUI(swarm, recreateSwarm);
    addModeControls(swarm);

    window.addEventListener("resize", () => {
        params.width = window.innerWidth;
        params.height = window.innerHeight;
        
        camera.left = params.width / -2;
        camera.right = params.width / 2;
        camera.top = params.height / 2;
        camera.bottom = params.height / -2;
        camera.updateProjectionMatrix();
        
        renderer.setSize(params.width, params.height);
    });

    function animate() {
        requestAnimationFrame(animate);
        
        if (swarm) swarm.update();
        
        renderer.render(scene, camera);
    }

    animate();
}

function addModeControls(swarm) {
    // Verificar si ya existe la sección
    if (document.querySelector('.mode-controls')) {
        return;
    }
    
    const modeSection = document.createElement('div');
    modeSection.className = 'section';
    modeSection.innerHTML = `
        <h3><i class="fas fa-sync-alt"></i> Modo de Enjambre</h3>
        <div class="button-group mode-controls" style="grid-template-columns: repeat(2, 1fr);">
            <button id="orbitalModeBtn" class="btn-secondary active" data-mode="orbital">
                <i class="fas fa-circle-notch"></i> Orbital
            </button>
            <button id="efficientModeBtn" class="btn-secondary" data-mode="efficient">
                <i class="fas fa-battery-full"></i> Eficiente
            </button>
        </div>
        <div style="margin-top: 10px; font-size: 12px; color: #888;">
            <i class="fas fa-info-circle"></i> 
            <span id="modeDescription">Drones no usados orbitan alrededor</span>
        </div>
    `;

    const figureSection = document.querySelector('.section:nth-child(1)');
    if (figureSection) {
        figureSection.parentNode.insertBefore(modeSection, figureSection.nextSibling);
    } else {
        document.querySelector('.panel-content').prepend(modeSection);
    }

    // Configurar event listeners
    setupModeButtons(swarm);
}

function setupModeButtons(swarm) {
    const orbitalBtn = document.getElementById('orbitalModeBtn');
    const efficientBtn = document.getElementById('efficientModeBtn');
    
    if (orbitalBtn) {
        orbitalBtn.onclick = () => {
            console.log('Cambiando a modo orbital...');
            swarm.setMode('orbital');
            updateModeButtons('orbital');
        };
    }
    
    if (efficientBtn) {
        efficientBtn.onclick = () => {
            console.log('Cambiando a modo eficiente...');
            swarm.setMode('efficient');
            updateModeButtons('efficient');
        };
    }
}

function updateModeButtons(activeMode) {
    document.querySelectorAll('.mode-controls button').forEach(btn => {
        btn.classList.remove('active');
    });
    
    const activeBtn = document.getElementById(`${activeMode}ModeBtn`);
    if (activeBtn) {
        activeBtn.classList.add('active');
    }
    
    const descriptions = {
        orbital: 'Drones no usados orbitan alrededor',
        efficient: 'Drones no usados se quedan ordenados abajo'
    };
    
    const descriptionEl = document.getElementById('modeDescription');
    if (descriptionEl) {
        descriptionEl.textContent = descriptions[activeMode] || '';
    }
}

// Función para reinicializar controles UI después de recrear swarm
function reinitializeUIControls(swarm) {
    console.log("Reinicializando controles UI...");
    
    // Reconfigurar botones de modo
    setupModeButtons(swarm);
    
    // Actualizar estadísticas inmediatamente
    if (typeof window.updateStats === 'function') {
        console.log("Llamando a updateStats...");
        window.updateStats(swarm);
    }
}