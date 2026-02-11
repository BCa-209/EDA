import cv2
import json

def exportar_matriz_json(ruta_imagen, nombre_salida="logo_sistemas.json", ancho_deseado=128):
    # 1. Cargar imagen en escala de grises
    img = cv2.imread(ruta_imagen, 0)
    if img is None:
        print("Error: No se pudo cargar la imagen.")
        return

    # 2. Redimensionar manteniendo proporción
    alto_orig, ancho_orig = img.shape
    proporcion = alto_orig / ancho_orig
    nuevo_alto = int(ancho_deseado * proporcion)
    img_redim = cv2.resize(img, (ancho_deseado, nuevo_alto))

    # 3. Detectar contornos
    bordes = cv2.Canny(img_redim, 100, 200)

    # 4. Convertir matriz a lista de strings de 0s y 1s
    lista_data = []
    for fila in bordes:
        # Convertimos cada fila en un string binario
        fila_str = "".join(["1" if pixel > 0 else "0" for pixel in fila])
        lista_data.append(fila_str)

    # 5. Crear estructura del JSON
    objeto_json = {
        "name": "diablada",
        "width": ancho_deseado,
        "height": nuevo_alto,
        "data": lista_data
    }

    # 6. Guardar archivo
    with open(nombre_salida, 'w') as f:
        json.dump(objeto_json, f, indent=2)
    
    print(f"Éxito: Matriz guardada en {nombre_salida}")

# Uso del script
exportar_matriz_json('Sistemas.png', ancho_deseado=128)