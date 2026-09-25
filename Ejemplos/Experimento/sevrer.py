#Arduino => Esp32 => Python Flask Server => App Inventor
from flask import Flask, request, jsonify # Importa lo  necesario de Flask para crear el servidor web
import json # Importa el módulo json para trabajar con datos en formato JSON 
import os # Importa os para interactuar con el sistema operativo 

app = Flask(__name__) # Creamos la aplicación Flask.

# Define el nombre del archivo donde se guardarán los últimos datos de telemetría.
DATA_FILE = 'data.json'

# Verifica si el archivo 'data.json' no existe en el directorio actual.
if not os.path.exists(DATA_FILE):
    # Si el archivo no existe, lo crea y lo inicializa con un diccionario JSON vacío o con valores predeterminados.
    # Esto asegura que siempre haya un archivo 'data.json' disponible para leer o escribir.
    with open(DATA_FILE, 'w') as f:
        json.dump({"distance": 0, "temperatura": 0, "humedad": 0, "light": 0, "sound": 0}, f, indent=4)

# Esta variable global almacenará los últimos datos de telemetría recibidos, se inicializa con valores predeterminados.
# Estos valores serán actualizados cada vez que se reciban nuevos datos del ESP32.
telemetry_data = {"distance": 0, "temperatura": 0, "humedad": 0, "light": 0, "sound": 0}

# --- Rutas del Servidor Web ---

@app.route('/') # Define la ruta raíz del servidor.
def home():
    # Devuelve un mensaje simple de texto para confirmar que el servidor está activo.
    # No se crea una página HTML compleja aquí para mantener la simplicidad y evitar errores otra vez.
    return "Servidor Flask en funcionamiento"#si está bien todo, eso dice en el puerto 5000 o el que se defina.

@app.route('/data', methods=['POST']) # Define la ruta '/data' que solo acepta peticiones POST. (nota: 'POST' es el método HTTP usado para enviar datos y pedirlos al servidor)
def recibir_datos():
    global telemetry_data # Declara que vamos a modificar la variable global 'telemetry_data'.

    # Verifica si la petición entrante tiene un encabezado 'Content-Type: application/json'.
    # Esto es crucial para asegurar que el cuerpo de la petición es un JSON válido.
    if request.is_json:
        # Si es JSON, manda el contenido la petición a un diccionario Python.
        sensor_data = request.get_json()

        # Extrae los datos individuales del diccionario JSON.
        # Los nombres de las claves deben coincidir exactamente con los nombres de las claves que el ESP32 envía en su JSON.
        distance = sensor_data.get('distance')
        temperatura = sensor_data.get('temperatura')
        humedad = sensor_data.get('humedad')
        light = sensor_data.get('light')
        sound = sensor_data.get('sound')

        # Actualiza la variable global 'telemetry_data' con los valores recién recibidos, esto asegura que 'telemetry_data' siempre contenga los datos más recientes.
        telemetry_data = {
            "distance": distance,
            "temperatura": temperatura,
            "humedad": humedad,
            "light": light,
            "sound": sound
        }

        # Imprime los datos recibidos en la consola del servidor. Esto es útil para depuración.
        print(f"Datos recibidos:")
        print(f"  Distancia: {distance} cm")
        print(f"  Temperatura: {temperatura} °C")
        print(f"  Humedad: {humedad} %")
        print(f"  Luz: {light}")
        print(f"  Sonido: {sound}")

        # Guarda los datos actuales en el archivo 'data.json'.
        with open(DATA_FILE, 'w') as f:
            json.dump(telemetry_data, f, indent=4) #esto lo hace un archivo JSON legible con indentación de 4 espacios.

        # Devuelve una respuesta JSON al cliente (ESP32) indicando que los datos fueron recibidos y guardados exitosamente.
        # El código de estado HTTP 200 significa "OK".
        return jsonify({"status": "success", "mensaje": "Datos recibidos y guardados"}), 200
    else:
        # Si la petición no tiene el formato JSON esperado, imprime un mensaje de error y devuelve una respuesta de error al cliente.
        # El código de estado HTTP 400 significa "Bad Request" (Solicitud incorrecta).
        print("No se recibió datos en formato JSON")
        return jsonify({"status": "error", "mensaje": "Formato de petición no válido"}), 400

@app.route('/obtener_data', methods=['GET']) # Define la ruta '/obtener_data' que solo acepta peticiones GET
def obtener_data():
    global telemetry_data # Declara que vamos a usar la variable global 'telemetry_data'.
    # Retorna el diccionario 'telemetry_data' como una respuesta JSON.
    # Los clientes consumirán este JSON para mostrar los datos en tiempo real.
    # El código de estado HTTP 200 significa "OK".
    return jsonify(telemetry_data), 200

# --- Inicio del Servidor ---
if __name__ == '__main__':
    # Este bloque se ejecuta solo cuando el script se corre directamente y hace lo siguiente:
    # Inicia el servidor Flask.
    # 'host='0.0.0.0'' hace que el servidor sea accesible desde cualquier dirección IP en la red local (no solo desde la propia máquina 'localhost'). Esto es esencial para que el ESP32 pueda conectarse.
    # 'port=5000' especifica el puerto en el que el servidor escuchará las peticiones, en mi caso el 5000
    app.run(host='0.0.0.0', port=5000)
