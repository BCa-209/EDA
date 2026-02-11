/* =========================
   UTILIDADES (CANVAS Y TEXTO)
========================= */

// Convierte lo dibujado en el canvas a puntos
export function canvasToPoints(canvas) {
    // Agregar willReadFrequently para mejor rendimiento
    const ctx = canvas.getContext("2d", { willReadFrequently: true });
    const img = ctx.getImageData(0, 0, canvas.width, canvas.height);
    
    let activePixels = 0;
    for (let i = 3; i < img.data.length; i += 4) {
      if (img.data[i] > 128) activePixels++;
    }
    
    let step = 6;
    if (activePixels < 6000) step = 3;
    if (activePixels < 3000) step = 2;
    
    const points = [];
    
    for (let y = 0; y < img.height; y += step) {
      for (let x = 0; x < img.width; x += step) {
        const i = (y * img.width + x) * 4;
        if (img.data[i + 3] > 128) {
          points.push({
            x: x - canvas.width / 2,
            y: canvas.height / 2 - y
          });
        }
      }
    }
    return points;
}

// Convierte una cadena de texto a puntos usando el canvas
export function textToPoints(text, canvas) {
    const ctx = canvas.getContext("2d");
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    const lines = text.split("\n").filter(line => line.trim() !== "");
    if (lines.length === 0) return [];
    
    const maxWidth = canvas.width * 0.9;
    const maxHeight = canvas.height * 0.9;
    
    let fontSize = 140;
    const lineSpacing = 1.2;
    
    ctx.textAlign = "center";
    ctx.textBaseline = "middle";
    ctx.fillStyle = "white";
    
    // Ajustar tamaño de fuente
    while (fontSize > 20) {
      ctx.font = `bold ${fontSize}px Arial`;
      const widestLine = Math.max(...lines.map(l => ctx.measureText(l).width));
      const totalHeight = lines.length * fontSize * lineSpacing;
      if (widestLine <= maxWidth && totalHeight <= maxHeight) break;
      fontSize--;
    }
    
    const startY = canvas.height / 2 - ((lines.length - 1) * fontSize * lineSpacing) / 2;
    
    lines.forEach((line, i) => {
      const y = startY + i * fontSize * lineSpacing;
      ctx.fillText(line, canvas.width / 2, y);
    });
    
    return canvasToPoints(canvas);
}