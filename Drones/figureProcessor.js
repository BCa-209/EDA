/* =========================
   PROCESADOR DE FIGURAS (JSON)
========================= */
import { figureConfigs } from './config.js';

const figureCache = new Map();

// 1. Procesamiento EXACTO
function processMatrix128Exact(matrixRows, scale = 2.0, spacing = 2.0) {
    const points = [];
    const rows = matrixRows.length;
    const cols = matrixRows[0]?.length || 0;
    const centerX = cols / 2;
    const centerY = rows / 2;
    const grid = new Set();
  
    for (let y = 0; y < rows; y++) {
      for (let x = 0; x < cols; x++) {
        if (matrixRows[y][x] === '1') {
          const screenX = (x - centerX) * scale;
          const screenY = (centerY - y) * scale;
          const key = `${Math.floor(screenX)},${Math.floor(screenY)}`;
          
          if (!grid.has(key)) {
            points.push({ x: screenX, y: screenY });
            grid.add(key);
          }
        }
      }
    }
    return points;
}

// 2. Procesamiento INTELIGENTE
function processMatrix128Intelligent(matrixRows, targetPoints = 1000, scale = 2.0, spacing = 2.0) {
    const points = [];
    const rows = matrixRows.length;
    const cols = matrixRows[0]?.length || 0;
    const centerX = cols / 2;
    const centerY = rows / 2;
    
    let totalOnes = 0;
    const onesPositions = [];
    
    for (let y = 0; y < rows; y++) {
      for (let x = 0; x < cols; x++) {
        if (matrixRows[y][x] === '1') {
          totalOnes++;
          onesPositions.push({ x, y });
        }
      }
    }
    
    let sampleFactor = 1;
    if (totalOnes > targetPoints) {
      sampleFactor = Math.max(1, Math.floor(totalOnes / targetPoints));
    }
    
    const grid = new Set();
    const gridSize = spacing;
    
    for (let i = 0; i < onesPositions.length; i += sampleFactor) {
      const { x, y } = onesPositions[i];
      const screenX = (x - centerX) * scale;
      const screenY = (centerY - y) * scale;
      const gridKey = `${Math.floor(screenX/gridSize)},${Math.floor(screenY/gridSize)}`;
      
      if (!grid.has(gridKey)) {
        points.push({ x: screenX, y: screenY });
        grid.add(gridKey);
        // Marcar vecinos para evitar aglomeración
        grid.add(`${Math.floor(screenX/gridSize)+1},${Math.floor(screenY/gridSize)}`);
        grid.add(`${Math.floor(screenX/gridSize)},${Math.floor(screenY/gridSize)+1}`);
      }
    }
    return points;
}

// 3. Procesamiento BORDES
function processMatrix128Borders(matrixRows, scale = 2.0) {
    const points = [];
    const rows = matrixRows.length;
    const cols = matrixRows[0]?.length || 0;
    const centerX = cols / 2;
    const centerY = rows / 2;
  
    for (let y = 0; y < rows; y++) {
      for (let x = 0; x < cols; x++) {
        if (matrixRows[y][x] === '1') {
            // Lógica simple de detección de bordes
            const isBorder = (x>0 && matrixRows[y][x-1] === '0') || 
                             (x<cols-1 && matrixRows[y][x+1] === '0') ||
                             (y>0 && matrixRows[y-1][x] === '0') ||
                             (y<rows-1 && matrixRows[y+1][x] === '0');

            if(isBorder) {
                const screenX = (x - centerX) * scale;
                const screenY = (centerY - y) * scale;
                points.push({ x: screenX, y: screenY });
            }
        }
      }
    }
    return points;
}

// Función Principal de Carga
export async function loadFigureFromJSON(figureName, droneCount) {
    const config = figureConfigs[figureName];
    if (!config) return getDefaultFigure(figureName);
  
    const cacheKey = `${figureName}_${droneCount}`;
    if (figureCache.has(cacheKey)) return figureCache.get(cacheKey);
  
    try {
      console.log(`Cargando ${config.name}...`);
      const response = await fetch(config.jsonFile);
      if (!response.ok) throw new Error("Error HTTP");
      
      const data = await response.json();
      const method = config.method || 'intelligent';
      let points;
      
      if (method === 'exact') points = processMatrix128Exact(data.data, config.scale, config.spacing);
      else if (method === 'borders') points = processMatrix128Borders(data.data, config.scale);
      else points = processMatrix128Intelligent(data.data, droneCount * 0.8, config.scale, config.spacing);
      
      // Ajuste automático de escala si es muy pequeño
      const bounds = getBounds(points);
      let finalPoints = points;
      let finalScale = config.scale;

      if ((bounds.maxX - bounds.minX) < 100 && points.length > 0) {
        finalScale = config.scale * 2.5;
        finalPoints = points.map(p => ({ x: p.x * 2.5, y: p.y * 2.5 }));
      }
  
      const figureData = {
        points: finalPoints,
        color: config.color,
        scale: finalScale,
        pointCount: finalPoints.length,
        name: config.name,
        originalScale: config.scale
      };
  
      figureCache.set(cacheKey, figureData);
      return figureData;
  
    } catch (error) {
      console.error(`Error cargando ${figureName}:`, error);
      return getDefaultFigure(figureName);
    }
}

function getBounds(points) {
    let minX = Infinity, maxX = -Infinity;
    points.forEach(p => {
        minX = Math.min(minX, p.x);
        maxX = Math.max(maxX, p.x);
    });
    return { minX, maxX };
}

export function getDefaultFigure(figureName) {
    // Retorna una figura vacía o básica si falla la carga
    return { points: [], color: 0xFFFFFF, name: figureName || 'Desconocido', scale: 1 };
}