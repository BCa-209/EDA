/* =========================
   CLASE SWARM (ENJAMBRE)
========================= */
import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.158.0/build/three.module.js';
import { Drone } from './drone.js';
import { params, figureConfigs } from './config.js';

export class Swarm {
  constructor(scene, count) {
    this.scene = scene;
    this.drones = [];
    this.orbitCenter = new THREE.Vector3(0, 0, 0);
    this.orbitTime = 0;
    this.currentPoints = [];
    
    // Grid para optimizar colisiones
    this.grid = new Map();
    this.cellSize = params.personalSpace * 3;
    
    this.init(count);
  }
  
  init(count) {
    for (let i = 0; i < count; i++) {
      const x = (Math.random() - 0.5) * params.width;
      const y = (Math.random() - 0.5) * params.height;
      const d = new Drone(x, y);
      this.drones.push(d);
      this.scene.add(d.mesh);
    }
    // Estado inicial: todos orbitando
    this.setFormation([], 0xffffff);
  }
  
  dispose() {
    this.drones.forEach(d => this.scene.remove(d.mesh));
    this.drones = [];
    this.currentPoints = [];
  }
  
  // Asigna objetivos (puntos) a los drones
  setFormation(points, color = 0xffffff, centerAtOrigin = true) {
    this.currentPoints = points;
    this.originalPoints = [...points];
    this.currentColor = color;

    // Centrar puntos si es necesario
    if (centerAtOrigin && points.length > 0) {
        const center = points.reduce((acc, p) => ({x: acc.x + p.x, y: acc.y + p.y}), {x:0, y:0});
        center.x /= points.length;
        center.y /= points.length;
        points = points.map(p => ({ x: p.x - center.x, y: p.y - center.y }));
        this.currentPoints = points;
    }

    // Calcular centro de órbita para los drones que sobran
    this.orbitCenter.set(0, 0, 0); 

    // Asignar tareas
    const dronesPerPoint = points.length > 0 ? Math.floor(this.drones.length / points.length) : 0;
    
    this.drones.forEach((drone, i) => {
        drone.originalColor = color; // Actualizar color base
        
        if (i < points.length * Math.max(1, dronesPerPoint)) {
            // Drone forma parte de la figura
            const p = points[i % points.length]; // Repartir si hay más drones que puntos
            drone.target.set(p.x, p.y, 0);
            drone.isOrbiting = false;
        } else {
            // Drone sobra -> va a órbita
            drone.isOrbiting = true;
        }
    });
  }
  
  update() {
    this.orbitTime += 0.05;
    this.updateGrid();
    
    // Actualizar objetivos de los que orbitan
    let orbitIndex = 0;
    this.drones.forEach(d => {
        if(d.isOrbiting) {
            this.updateOrbitalTarget(d, orbitIndex++);
        }
        // Buscar vecinos y actualizar física
        const neighbors = this.getNeighbors(d);
        d.update(neighbors);
    });
  }
  
  updateOrbitalTarget(drone, index) {
      const radiusVar = (index % 8) * 25;
      const speedVar = (index % 13) * 0.003;
      const r = params.baseRadius + radiusVar;
      const angle = this.orbitTime * (params.baseOrbitSpeed + speedVar) + index * 0.15;
      
      drone.target.x = this.orbitCenter.x + Math.cos(angle) * r;
      drone.target.y = this.orbitCenter.y + Math.sin(angle) * r;
  }

  // --- Grid Espacial para optimización ---
  updateGrid() {
    this.grid.clear();
    this.cellSize = params.personalSpace * 3; // Actualizar por si cambió el slider
    
    this.drones.forEach(d => {
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
    
    for(let x = -1; x <= 1; x++) {
        for(let y = -1; y <= 1; y++) {
            const cell = this.grid.get(`${cx+x},${cy+y}`);
            if(cell) {
                // Agregar contenido de la celda a vecinos
                for(let other of cell) neighbors.push(other);
            }
        }
    }
    return neighbors;
  }
  
  // Datos para la UI
  getStats() {
      const active = this.drones.filter(d => !d.isOrbiting).length;
      return {
          total: this.drones.length,
          formation: active,
          orbiting: this.drones.length - active
      };
  }
}