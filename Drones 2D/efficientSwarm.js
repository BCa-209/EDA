import * as THREE from "https://cdn.jsdelivr.net/npm/three@0.158.0/build/three.module.js";
import { Drone } from "./drone.js";
import { params } from "./config.js";

export class EfficientSwarm {
    constructor(scene, count) {
        this.scene = scene;
        this.drones = [];
        this.count = count;
        this.activeDrones = [];
        this.currentPoints = [];
        this.currentColor = 0xffffff;
        this.currentFigureName = null;
        this.formationColor = 0xffffff;

        // Grid para colisiones
        this.grid = new Map();
        this.cellSize = params.personalSpace * 3;
    }

    init() {
        this.dispose(); // Limpiar cualquier dron existente

        // Configuración de la cuadrícula en filas
        const dronesPerRow = 25;
        const spacing = 12;
        const startY = -300; // Posicionado en parte baja

        for (let i = 0; i < this.count; i++) {
            const row = Math.floor(i / dronesPerRow);
            const col = i % dronesPerRow;

            const x = (col - dronesPerRow / 2) * spacing;
            const y = startY - row * spacing;

            const drone = new Drone(x, y);

            // Estado inicial
            drone.mesh.userData.isDrone = true; // Marcar para limpieza eficiente
            drone.homePosition = new THREE.Vector3(x, y, 0);
            drone.isActive = false;
            drone.isOrbiting = false; // No orbitan en modo eficiente
            drone.target.copy(drone.homePosition);

            this.drones.push(drone);
            this.scene.add(drone.mesh);
        }
    }

    dispose() {
        const dronesToRemove = [];

        this.drones.forEach((d) => {
            if (d.mesh && this.scene) {
                dronesToRemove.push(d.mesh);
            }
        });

        dronesToRemove.forEach(mesh => {
            if (this.scene.children.includes(mesh)) {
                this.scene.remove(mesh);
                if (mesh.geometry) mesh.geometry.dispose();
                if (mesh.material) mesh.material.dispose();
            }
        });

        this.drones = [];
        this.activeDrones = [];
        this.currentPoints = [];
        this.currentFigureName = null;
        this.grid.clear();
        
        if (typeof gc !== "undefined") gc();
    }
    
    setFormation(points, color = 0xffffff) {
        this.currentFigureName = null;
        this.currentPoints = points;
        this.currentColor = color;
        this.formationColor = color;
        this.activeDrones = [];

        // Resetear todos los drones
        this.drones.forEach((d) => {
            d.isActive = false;
            d.isOrbiting = false;
            d.target.copy(d.homePosition);
        });

        // Activar solo los necesarios
        const dronesNeeded = Math.min(points.length, this.drones.length);

        for (let i = 0; i < dronesNeeded; i++) {
            const drone = this.drones[i];
            drone.isActive = true;
            drone.originalColor = color;
            drone.target.set(points[i].x, points[i].y, 0);
            this.activeDrones.push(drone);
        }
    }

    update() {
        this.updateGrid();

        // Solo drones activos hacen update completo con colisiones
        this.activeDrones.forEach((d) => {
            const neighbors = this.getNeighbors(d);
            d.update(neighbors);
        });

        // Drones inactivos: movimiento mínimo para mantener posición
        this.drones.forEach((d) => {
            if (!d.isActive) {
                const diff = new THREE.Vector3().subVectors(
                    d.homePosition,
                    d.mesh.position,
                );
                if (diff.length() > 1) {
                    // Movimiento suave hacia posición de reposo
                    d.velocity.add(diff.normalize().multiplyScalar(0.03));
                    d.velocity.multiplyScalar(0.95);
                    d.mesh.position.add(d.velocity);
                }

                // Aún verifican colisiones en reposo
                const neighbors = this.getNeighbors(d);
                if (params.collisionsEnabled) {
                    this.resolveIdleCollisions(d, neighbors);
                }
            }
        });
    }

    // === COLISIONES COMPARTIDAS ===
    updateGrid() {
        this.grid.clear();
        this.cellSize = params.personalSpace * 3;

        this.drones.forEach((d) => {
            const cx = Math.floor(d.mesh.position.x / this.cellSize);
            const cy = Math.floor(d.mesh.position.y / this.cellSize);
            const key = `${cx},${cy}`;
            if (!this.grid.has(key)) this.grid.set(key, []);
            this.grid.get(key).push(d);
        });
    }

    getNeighbors(drone) {
        const neighbors = [];
        const cx = Math.floor(drone.mesh.position.x / this.cellSize);
        const cy = Math.floor(drone.mesh.position.y / this.cellSize);

        for (let x = -1; x <= 1; x++) {
            for (let y = -1; y <= 1; y++) {
                const cell = this.grid.get(`${cx + x},${cy + y}`);
                if (cell) {
                    for (let other of cell) neighbors.push(other);
                }
            }
        }
        return neighbors;
    }

    // Método específico para colisiones en reposo
    resolveIdleCollisions(drone, neighbors) {
        let push = new THREE.Vector3();
        let count = 0;

        for (const n of neighbors) {
            if (n === drone) continue;
            const diff = new THREE.Vector3().subVectors(
                drone.mesh.position,
                n.mesh.position,
            );
            const dist = diff.length();

            if (dist < params.personalSpace && dist > 0) {
                const force = Math.pow(
                    (params.personalSpace - dist) / params.personalSpace,
                    2,
                );
                push.add(
                    diff.normalize().multiplyScalar(force * params.separationForce * 0.3),
                );
                count++;
            }
        }

        if (count > 0) {
            push.divideScalar(count);
            const maxPush = 0.15;
            if (push.length() > maxPush) {
                push.normalize().multiplyScalar(maxPush);
            }
            drone.velocity.add(push);
        }
    }

    getStats() {
        return {
            total: this.drones.length,
            formation: this.activeDrones.length,
            idle: this.drones.length - this.activeDrones.length,
            mode: "Eficiente",
        };
    }
}
