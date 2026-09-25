import cv2
import serial
import time
import numpy as np

# --- CONFIGURACIÓN DE ARDUINO CON PYSERIAL ---
# ¡IMPORTANTE! Reemplaza 'COMX' con tu puerto serial ('COM8')
# El baudrate debe coincidir con el del código de Arduino (9600)
SERIAL_PORT = 'COM8' 
BAUDRATE = 9600

try:
    # Intenta abrir el puerto serial
    arduino_serial = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)
    time.sleep(2)  # Espera para establecer la conexión
    print(f"Conexión con Arduino establecida en {SERIAL_PORT}.")
except Exception as e:
    print(f"Error al conectar con Arduino: {e}")
    print("Asegúrate de que el puerto serial sea correcto y el Arduino esté conectado.")
    exit()

# --- CONFIGURACIÓN DE CÁMARA Y DETECCIÓN ---
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Error: No se puede abrir la cámara.")
    arduino_serial.close()
    exit()

# Definir la Zona de Interés (ROI) y dimensiones
roi_size = 100
frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

x_start = (frame_width // 2) - (roi_size // 2)
y_start = (frame_height // 2) - (roi_size // 2)
x_end = x_start + roi_size
y_end = y_start + roi_size

# Umbral de activación: Ajusta este valor según la iluminación de tu entorno
ACTIVATION_THRESHOLD = 90 

print(f"Zona de detección: Centro de {roi_size}x{roi_size} píxeles.")
print(f"Umbral de activación: {ACTIVATION_THRESHOLD}. Presiona 'q' para salir.")


while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    # 1. Obtener la Zona de Interés (ROI)
    roi = frame[y_start:y_end, x_start:x_end]
    gray_roi = cv2.cvtColor(roi, cv2.COLOR_BGR2GRAY)
    average_brightness = np.mean(gray_roi)

    # 2. Lógica de Control
    command = ''
    if average_brightness > ACTIVATION_THRESHOLD:
        command = 'H'  # High - Encender
        color = (0, 255, 0)  # Verde
        cv2.putText(frame, "LED ON", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, color, 2)
    else:
        command = 'L'  # Low - Apagar
        color = (0, 0, 255)  # Rojo
        cv2.putText(frame, "LED OFF", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, color, 2)

    # 3. Enviar el comando al Arduino (se envía un byte)
    arduino_serial.write(command.encode('utf-8'))

    # Dibujar la ROI y el texto
    cv2.rectangle(frame, (x_start, y_start), (x_end, y_end), color, 3)
    cv2.putText(frame, f"Brillo: {average_brightness:.2f}", (x_start, y_end + 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, color, 2)
    
    cv2.imshow('OpenCV PySerial Control', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Liberar recursos y cerrar el puerto serial
arduino_serial.write('L'.encode('utf-8')) # Apagar el LED al cerrar
arduino_serial.close()
cap.release()
cv2.destroyAllWindows()