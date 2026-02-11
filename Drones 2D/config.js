/* =========================
   ESTADO GLOBAL Y CONFIGURACIÓN
========================= */
export const params = {
    width: window.innerWidth,
    height: window.innerHeight,
    droneSpeed: 0.07,
    droneCount: 800,
    baseRadius: 120,
    baseOrbitSpeed: 0.015,
    collisionsEnabled: true,
    personalSpace: 8,
    separationForce: 0.5,
    currentFigure: null
};

// Configuración de las figuras JSON
export const figureConfigs = {
    'mascara': { 
        jsonFile: 'figures/mascara.json', 
        scale: 3.0, spacing: 3.0, color: 0xFF5733, 
        name: 'Máscara', method: 'intelligent' 
    },
    'logo_escuela': { 
        jsonFile: 'figures/logo_escuela.json', 
        scale: 3.0, spacing: 3.0, color: 0x33A1FF, 
        name: 'Logo Escuela', method: 'intelligent' 
    },
    'logo_universidad': {   
        jsonFile: 'figures/logo_universidad.json', 
        scale: 3.0, spacing: 3.0, color: 0x9D33FF, 
        name: 'Logo Universidad', method: 'intelligent' 
    },
    'estrella': { 
        jsonFile: 'figures/estrella.json', 
        scale: 3.0, spacing: 3.0, color: 0xFFFF33, 
        name: 'Estrella', method: 'intelligent' 
    }
};