import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.158.0/build/three.module.js';
import { Swarm } from './swarm.js';
import { EfficientSwarm } from './efficientSwarm.js';

export class SwarmSystem {
  constructor(scene, count) {
    this.scene = scene;
    this.drones = [];
    this.mode = 'orbital';
    this.activeMode = null;
    this.count = count;
    this.formationColor = 0xffffff;
    
    // Guardar la formación actual para restaurarla
    this.currentFormation = {
      points: [],
      color: 0xffffff,
      figureName: null
    };
    
    this.modes = {};
    
    this.setMode('orbital');
  }
  
  setMode(mode) {
    if (!['orbital', 'efficient'].includes(mode)) {
      mode = 'orbital';
    }
    
    if (this.mode === mode && this.activeMode) {
      return;
    }
    
    console.log(`Cambiando de modo ${this.mode} a ${mode}...`);
    
    // Guardar formación actual del modo anterior
    if (this.activeMode) {
      this.currentFormation = {
        points: this.activeMode.currentPoints || [],
        color: this.activeMode.currentColor || 0xffffff,
        figureName: this.activeMode.currentFigureName || null
      };
      
      this.activeMode.dispose();
      this.activeMode = null;
      this.drones = [];
    }
    
    // Crear nuevo modo si no existe
    if (!this.modes[mode]) {
      if (mode === 'orbital') {
        this.modes[mode] = new Swarm(this.scene, this.count);
      } else if (mode === 'efficient') {
        this.modes[mode] = new EfficientSwarm(this.scene, this.count);
      }
    }
    
    // Actualizar referencia
    this.mode = mode;
    this.activeMode = this.modes[mode];
    
    // Inicializar el modo
    this.activeMode.init();
    this.drones = this.activeMode.drones;
    
    // Restaurar formación guardada
    if (this.currentFormation.points.length > 0) {
      console.log(`Restaurando formación: ${this.currentFormation.points.length} puntos`);
      this.activeMode.setFormation(
        this.currentFormation.points, 
        this.currentFormation.color
      );
      if (this.currentFormation.figureName) {
        this.activeMode.currentFigureName = this.currentFormation.figureName;
      }
    }
    
    console.log(`Modo cambiado a: ${mode}`);
  }
  
setFormation(points, color = 0xffffff, figureName = null) {
    console.log("SwarmSystem.setFormation llamado");
    console.log("activeMode disponible:", this.activeMode ? "Sí" : "No");
    
    if (!this.activeMode) {
        console.error("ERROR: activeMode es null! Inicializando modo actual...");
        // Intentar inicializar el modo actual
        this.setMode(this.mode || 'orbital');
        
        if (!this.activeMode) {
            console.error("ERROR CRÍTICO: No se pudo inicializar activeMode");
            return;
        }
    }
    
    console.log("Puntos:", points.length);
    console.log("Color:", color.toString(16));
    console.log("Nombre figura:", figureName);
    
    this.activeMode.setFormation(points, color);
    
    // Actualizar formación guardada
    this.currentFormation = {
        points: [...points],
        color: color,
        figureName: figureName
    };
    
    // Actualizar también formationColor
    this.formationColor = color;
    
    if (figureName) {
        this.activeMode.currentFigureName = figureName;
        console.log("currentFigureName establecido a:", figureName);
    }
    
    console.log("Formación guardada exitosamente");
}
  
  update() {
    if (this.activeMode) {
      this.activeMode.update();
    }
  }
  
  dispose() {
    Object.values(this.modes).forEach(mode => {
      if (mode && mode.dispose) {
        mode.dispose();
      }
    });
    this.modes = {};
    this.activeMode = null;
    this.drones = [];
    this.currentFormation = {
      points: [],
      color: 0xffffff,
      figureName: null
    };
  }
  
  getStats() {
    return this.activeMode ? this.activeMode.getStats() : { 
      total: 0, 
      formation: 0, 
      mode: 'None' 
    };
  }
  
  get currentPoints() {
    return this.activeMode ? this.activeMode.currentPoints || [] : [];
  }
  
  // Nuevo método para obtener figura actual
  get currentFigureName() {
    return this.activeMode ? this.activeMode.currentFigureName || null : null;
  }


  // Añade este método a la clase SwarmSystem
checkState() {
    console.log("=== ESTADO SWARM SYSTEM ===");
    console.log("Modo actual:", this.mode);
    console.log("activeMode:", this.activeMode ? "Instanciado" : "NULL");
    console.log("Modos disponibles:", Object.keys(this.modes));
    console.log("Figura actual:", this.currentFigureName);
    console.log("Formación guardada:", this.currentFormation.points.length, "puntos");
    
    if (this.activeMode) {
        console.log("activeMode.currentFigureName:", this.activeMode.currentFigureName);
        console.log("activeMode.currentPoints:", this.activeMode.currentPoints?.length || 0);
    }
}
}