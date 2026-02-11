/* =========================
   MAIN (PUNTO DE ENTRADA)
========================= */
// Importa Three.js desde CDN directamente
import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.158.0/build/three.module.js';

// Los otros imports relativos funcionan
import { Swarm } from './swarm.js';
import { params } from './config.js';
import { setupUI } from './ui.js';

// Exporta una función init para ser llamada desde index.html
export function init() {
    // 1. Configuración de Escena
    const scene = new THREE.Scene();

    // Cámara Ortográfica (2D look)
    const camera = new THREE.OrthographicCamera(
        params.width / -2, params.width / 2,
        params.height / 2, params.height / -2, 
        0.1, 1000
    );
    camera.position.z = 10;

    const renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(params.width, params.height);
    renderer.setClearColor(0x000000);
    document.body.appendChild(renderer.domElement);

    // 2. Variable para la instancia del enjambre
    let swarm = new Swarm(scene, params.droneCount);

    // 3. Callback para recrear el enjambre
    const recreateSwarm = (newCount) => {
        swarm.dispose();
        swarm = new Swarm(scene, newCount);
        console.log("Enjambre reiniciado con " + newCount + " drones.");
    };

    // 4. Inicializar UI
    setupUI(swarm, recreateSwarm);

    // 5. Manejo de redimensionamiento
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

    // 6. Loop de Animación
    function animate() {
        requestAnimationFrame(animate);
        
        if (swarm) swarm.update();
        
        renderer.render(scene, camera);
    }

    animate();
}