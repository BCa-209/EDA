/* =========================
   CLASE DRONE
========================= */
import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.158.0/build/three.module.js';
import { params } from './config.js';

export class Drone {
  constructor(x, y, color = 0xffffff) {
    const g = new THREE.CircleGeometry(2, 10);
    const m = new THREE.MeshBasicMaterial({ color: color });
    this.mesh = new THREE.Mesh(g, m);
    this.mesh.position.set(x, y, 0);
    
    this.mesh.userData = { isDrone: true }; // Marcar para limpieza eficiente
    
    this.target = new THREE.Vector3(x, y, 0);
    this.velocity = new THREE.Vector3((Math.random()-0.5)*0.2, (Math.random()-0.5)*0.2, 0);
    
    // Propiedades simplificadas (sin híbrido)
    this.isOrbiting = false;
    this.isActive = false;          // Solo para modo eficiente
    
    // HomePosition solo para modo eficiente
    if (Math.abs(y) > 200) { // Si está en la parte baja, probablemente modo eficiente
      this.homePosition = new THREE.Vector3(x, y, 0);
    }
    
    this.originalColor = color;
    this.orbitColor = 0x00ffff;
    this.colorTransition = 0;
    this.pulsePhase = Math.random() * Math.PI * 2;
    
    // Parámetros internos
    this.maxSpeed = 1.5;
    this.alignmentFactor = 0.1;
    this.cohesionFactor = 0.05;
  }
  
  update(neighbors) {
    // 1. Colisiones
    if (params.collisionsEnabled) {
      this.resolveCollisions(neighbors);
      if (this.isOrbiting && !this.isActive) this.applyFlocking(neighbors);
    }
    
    // 2. Movimiento
    this.moveToTarget();
    this.mesh.position.add(this.velocity);
    this.velocity.multiplyScalar(0.5); // Fricción
    
    // 3. Visual (Color y Pulso)
    this.updateVisuals();

    // 4. Límites de pantalla
    this.stayInBounds();
  }
  
  resolveCollisions(neighbors) {
    let push = new THREE.Vector3();
    let count = 0;
    
    for (const n of neighbors) {
      if (n === this) continue;
      const diff = new THREE.Vector3().subVectors(this.mesh.position, n.mesh.position);
      const dist = diff.length();
      
      if (dist < params.personalSpace && dist > 0) {
        const force = Math.pow((params.personalSpace - dist) / params.personalSpace, 2);
        push.add(diff.normalize().multiplyScalar(force * params.separationForce * 0.5));
        count++;
      }
    }
    if (count > 0) {
      push.divideScalar(count);
      const maxPush = 0.3;
      if (push.length() > maxPush) {
        push.normalize().multiplyScalar(maxPush);
      }
      this.velocity.add(push);
    }
  }

  applyFlocking(neighbors) {
    if (neighbors.length === 0) return;
    
    let avgVel = new THREE.Vector3();
    let count = 0;
    neighbors.forEach(n => {
      if(n.isOrbiting && n !== this) {
        avgVel.add(n.velocity);
        count++;
      }
    });
    if(count > 0) {
      avgVel.divideScalar(count);
      this.velocity.add(avgVel.sub(this.velocity).multiplyScalar(this.alignmentFactor));
    }
  }
  
  moveToTarget() {
    const diff = new THREE.Vector3().subVectors(this.target, this.mesh.position);
    const dist = diff.length();
    
    if (dist > 0.5) {
      const speedFactor = params.droneSpeed * 0.5;
      const steer = diff.normalize().multiplyScalar(Math.min(this.maxSpeed, dist * speedFactor));
      this.velocity.add(steer);
    }
  }

  updateVisuals() {
    // Color
    this.colorTransition = this.isOrbiting ? 
      Math.min(this.colorTransition + 0.05, 1) : 
      Math.max(this.colorTransition - 0.05, 0);
      
    const c1 = new THREE.Color(this.originalColor);
    const c2 = new THREE.Color(this.orbitColor);
    this.mesh.material.color.copy(c1.lerp(c2, this.colorTransition));

    // Pulso (solo si orbita)
    if (this.isOrbiting) {
      this.pulsePhase += 0.05;
      this.mesh.scale.setScalar(Math.sin(this.pulsePhase) * 0.2 + 1);
    } else {
      this.mesh.scale.setScalar(1);
    }
  }
  
  stayInBounds() {
    const margin = 100;
    const limitX = params.width / 2 - margin;
    const limitY = params.height / 2 - margin;
    
    if (this.mesh.position.x < -limitX) this.velocity.x += 0.5;
    if (this.mesh.position.x > limitX)  this.velocity.x -= 0.5;
    if (this.mesh.position.y < -limitY) this.velocity.y += 0.5;
    if (this.mesh.position.y > limitY)  this.velocity.y -= 0.5;
  }
}