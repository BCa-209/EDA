import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.158.0/build/three.module.js';
import { Drone } from './drone.js';
import { params } from './config.js';

export class Swarm {
  constructor(scene, count) {
    this.scene = scene;
    this.drones = [];
    this.count = count;
    this.orbitCenter = new THREE.Vector3(0, 0, 0);
    this.orbitTime = 0;
    this.currentPoints = [];
    this.currentColor = 0xffffff;
    this.currentFigureName = null;
    this.formationColor = 0xffffff;
    
    // Grid para optimizar colisiones
    this.grid = new Map();
    this.cellSize = params.personalSpace * 3;
  }
  
  init() {
    console.log(`Inicializando modo orbital con ${this.count} drones...`);
    // Limpiar cualquier dron existente primero
    this.dispose();
    
    for (let i = 0; i < this.count; i++) {
      const x = (Math.random() - 0.5) * params.width;
      const y = (Math.random() - 0.5) * params.height;
      const d = new Drone(x, y);
      d.mesh.userData.isDrone = true; // Marcar para limpieza eficiente
      d.isOrbiting = true;

      this.drones.push(d);
      this.scene.add(d.mesh);
      }
    console.log(`${this.drones.length} drones creados en modo orbital`);
  }
  
  dispose() {

    const dronesToRemove = [];

    // Remover todos los drones de la escena
    this.drones.forEach(d => {
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
    this.currentPoints = [];
    this.currentFigureName = null;
    this.grid.clear();
    
    // Forzar garbage collection
    if (typeof gc !== 'undefined') gc();
  }
  
  setFormation(points, color = 0xffffff, figureName = null) {
    this.currentPoints = points;
    this.currentColor = color;
    this.currentFigureName = figureName;
    
    if (points.length > 0) {
      const center = points.reduce((acc, p) => ({x: acc.x + p.x, y: acc.y + p.y}), {x:0, y:0});
      center.x /= points.length;
      center.y /= points.length;
      points = points.map(p => ({ x: p.x - center.x, y: p.y - center.y }));
      this.currentPoints = points;
    }

    this.orbitCenter.set(0, 0, 0);

    const dronesPerPoint = points.length > 0 ? Math.floor(this.drones.length / points.length) : 0;
    
    this.drones.forEach((drone, i) => {
      drone.originalColor = color;
      
      if (i < points.length * Math.max(1, dronesPerPoint)) {
        const p = points[i % points.length];
        drone.target.set(p.x, p.y, 0);
        drone.isOrbiting = false;
      } else {
        drone.isOrbiting = true;
      }
    });
  }
  
  update() {
    this.orbitTime += 0.05;
    this.updateGrid();
    
    let orbitIndex = 0;
    this.drones.forEach(d => {
      if (d.isOrbiting) {
        this.updateOrbitalTarget(d, orbitIndex++);
      }
      
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
  
  updateGrid() {
    this.grid.clear();
    this.cellSize = params.personalSpace * 3;
    
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
          for(let other of cell) neighbors.push(other);
        }
      }
    }
    return neighbors;
  }
  
  getStats() {
    const active = this.drones.filter(d => !d.isOrbiting).length;
    return {
      total: this.drones.length,
      formation: active,
      orbiting: this.drones.length - active,
      mode: 'Orbital'
    };
  }
}