#include <WiFi.h>      // Esta es para conectarnos a internet con el ESP32
#include <HTTPClient.h>  // Para poder enviar datos a un servidor web
#include <ArduinoJson.h> // Para crear datos con formato JSON (como un diccionario)
#include <Wire.h>      // esta está aquí pero por unos problemitas (se rompieron los sensores que ocupan esas librerías), ya no se usa en el código


const char* ssid = "compu";     // Aquí pones el nombre del Wi-Fi. En mi caso es el hotspot de mi compu
const char* password = "12345678";  // Y aquí va la clave

const char* serverAddress = "http://192.168.1.70:5000/data"; // La dirección de nuestro servidor de python en vs code

#define RXD2 16  // Por este pin "recibimos" los datos que nos manda el Arduino
#define TXD2 17  // Y por este "mandamos" datos al Arduino, pero en este código no lo usamos, simplemente es para lo de comuncación serial con el Tx y Rx

const int SOUND_PIN = 34; // Este pin recibe la señal del micrófono
int soundVal = 0;         // Aquí guardamos el valor que lee el micrófono

//VARIABLES PARA GUARDAR LOS DATOS DEL ARDUINO 
String arduinoDataString = ""; // Aquí se guarda toda la línea de datos del Arduino (distancia, temperatura, etc.). (Nota: el "String", nos permite guardar esos datos con el valor de su variable que le asignemos o consiga la placa)
int distanceCm = 0;           // Distancia
float temperaturaC = 0.0;     // Temperatura
float humedad = 0.0;          // Humedad
int lightValue = 0;           // Nivel de luz

// TEMPORIZADOR PARA EL ENVÍO DE DATOS (lo mismo que en el otro con la placa arduino)
unsigned long ultimaLectura = 0;         // El tiempo de la última vez que enviamos datos
const long intervaloLectura = 500;       // Cada cuánto tiempo queremos enviar los datos al servidor (medio segundo)

void setup() {
  Serial.begin(115200);                  // otra vez, misma razón que con el de arduino
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); // Para que se comunique con el Arduino a la misma velocidad

  // Intentamos conectarnos al Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a Wi-Fi");

  // Si no se ha conectado, esperamos un poco
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");//pone esto cada segundo (1000) que pase intentando pero no conectándose
  }

  //Si llegamos aquí, es que se conectó y mostramos la IP
  Serial.println("\nWi-Fi conectado!");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  unsigned long tiempoActual = millis(); // Pregunta cuánto tiempo lleva prendido el ESP32

  // Revisamos si ya pasaron 500 milisegundos desde la última vez que enviamos datos
  if (tiempoActual - ultimaLectura >= intervaloLectura) {
    ultimaLectura = tiempoActual; //Guardamos el tiempo de ahora para la próxima vez

    // Si hay algo que el Arduino nos está mandando...
    if (Serial2.available()) {
      //leemos toda la línea de datos, hasta que encuentre el "enter" que pusimos en el Arduino
      arduinoDataString = Serial2.readStringUntil('\n');
      arduinoDataString.trim(); // Limpiamos cualquier espacio extra
      Serial.print("Recibido de Arduino: ");
      Serial.println(arduinoDataString);

      // Ahora, dividimos esa línea de datos que recibimos
      int firstComma = arduinoDataString.indexOf(','); // Buscamos la primera coma
      int secondComma = arduinoDataString.indexOf(',', firstComma + 1); // Buscamos la segunda coma
      int thirdComma = arduinoDataString.indexOf(',', secondComma + 1); // Buscamos la tercera coma
      
      // Y usamos esas comas para separar los datos y guardarlos en sus cajitas
      distanceCm = arduinoDataString.substring(0, firstComma).toInt(); // La distancia (el número antes de la primera coma)
      temperaturaC = arduinoDataString.substring(firstComma + 1, secondComma).toFloat(); // La temperatura (entre la primera y la segunda coma)
      humedad = arduinoDataString.substring(secondComma + 1, thirdComma).toFloat(); // La humedad (entre la segunda y la tercera)
      lightValue = arduinoDataString.substring(thirdComma + 1).toInt(); // Y la luz (después de la última coma)
    }

    // LECTURA DEL MICRÓFONO 
    soundVal = analogRead(SOUND_PIN); // Simplemente leemos el valor de sonido

    // Creamos un paquete de datos tipo JSON con un tamaño de 512 bytes, que es suficiente para lo que ocupamos
    StaticJsonDocument<512> doc; 
    // Ponemos todos los datos en el paquete con nombres fáciles de entender
    doc["distance"] = distanceCm;
    doc["temperatura"] = temperaturaC;
    doc["humedad"] = humedad;
    doc["light"] = lightValue;
    doc["sound"] = soundVal;

    String jsonPayload; // Creamos una caja para guardar el paquete de datos en texto
    serializeJson(doc, jsonPayload); //Y lo guardamos en esa caja

    Serial.print("JSON Payload: ");
    Serial.println(jsonPayload);

    // ENVÍO AL SERVIDOR DE PYTHON 
    // Si estamos conectados a internet...
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http; // Creamos un "mensajero" para enviar el paquete
      http.begin(serverAddress); // Le decimos a dónde debe ir el paquete, en este caso al server de python
      http.addHeader("Content-Type", "application/json"); // Le ponemos una etiqueta al paquete que dice que es un JSON

      //Y enviamos el paquete
      int httpResponseCode = http.POST(jsonPayload);

      // Revisamos si el envío fue exitoso
      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode); // Si el número es bueno, todo salió bien
      } else {
        Serial.print("Error en el envio HTTP: ");
        Serial.println(httpResponseCode); // Si es 0 o un número negativo, algo falló
      }
      
      http.end(); // El mensajero se va, y... FIN!
    }
  }
}


