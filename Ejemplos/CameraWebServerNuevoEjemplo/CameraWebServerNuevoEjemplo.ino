#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>  // *** CORRECCIÓN: Usamos ESP32Servo (o solo Servo.h) ***
#include "board_config.h"
Servo servoX;    // Eje X (Garra/Base): 0-170 grados
Servo servoIzq;  // Eje Izquierdo (Movimiento del Robot)
Servo servoDer;  // Eje Derecho (Movimiento del Robot)

#define SERVO_X_PIN 4
#define SERVO_IZQ_PIN 12
#define SERVO_DER_PIN 13
#define FLASH_GPIO_NUM 33

// Estado actual (para mantener la posición y evitar saltos)
int servoXPos = 90;
int servoIzqPos = 1;
int servoDerPos = 100;
bool flashOn = false;


// CONFIGURACIÓN DE WIFI

const char *ssid = "compu";
const char *password = "12345678";

// Declaraciones de funciones del servidor web (del código original)
void startCameraServer();
extern WebServer server;  // Accedemos al servidor web ya iniciado
WebServer server(80);


// FUNCIONES DE MOVIMIENTO

// 1. Eje X de la Cámara/Garra (Rango 0-170)
void moveServoX(int pos) {
  if (pos < 0) pos = 0;
  if (pos > 170) pos = 170;

  servoXPos = pos;
  servoX.write(servoXPos);
  Serial.printf("Servo X (Garra/Base) movido a: %d\n", servoXPos);
}

void moveRobot(int izqPos, int derPos) {
  // Aquí puedes implementar tus funciones 'abajo' y 'arriba' si solo controlan la garra.
  servoIzqPos = izqPos;
  servoDerPos = derPos;

  // Ejemplo simple: Ambos servos se mueven al mismo valor (pos) para un comando.
  servoIzq.write(izqPos);
  servoDer.write(derPos);

  Serial.printf("Robot movido: IZQ=%d, DER=%d\n", izqPos, derPos);
}

//MANEJADOR DE COMANDOS WEB

void handleControl() {
  // CONTROL DE SERVOS (garraX, camY)
  if (server.hasArg("servo") && server.hasArg("pos")) {
    String servoId = server.arg("servo");
    int pos = server.arg("pos").toInt();

    if (servoId == "garraX") {
      moveServoX(pos);  // Eje X (0-170)
      server.send(200, "text/plain", "OK: Servo X move");
      return;
    }
    // Si tienes un segundo servo de cámara (eje Y), añadirías 'camY' aquí
    // else if (servoId == "camY") {
    //   moveRobot(pos, pos); // Usando 'robotMove' para los otros dos servos (ejemplo)
    //   server.send(200, "text/plain", "OK: Servo Y move");
    //   return;
    // }
  }

  // CONTROL DE FLASH
  if (server.hasArg("flash")) {
    int state = server.arg("flash").toInt();  // 1 para ON, 0 para OFF
    flashOn = (state == 1);
    digitalWrite(FLASH_GPIO_NUM, flashOn ? HIGH : LOW);
    Serial.printf("Flash/Linterna: %s\n", flashOn ? "ON" : "OFF");
    server.send(200, "text/plain", "OK: Flash toggle");
    return;
  }

  // CONTROL DE MOVIMIENTO GENERAL (Si lo necesitas)
  // if (server.hasArg("action")) { ... }

  server.send(400, "text/plain", "Bad Request");
}


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  // 1. Configuración de Pines y Servos
  pinMode(FLASH_GPIO_NUM, OUTPUT);
  digitalWrite(FLASH_GPIO_NUM, LOW);

  // Configurar Servos (¡CORRECCIÓN! Solo se necesita el pin)
  servoX.attach(SERVO_X_PIN);
  servoIzq.attach(SERVO_IZQ_PIN);
  servoDer.attach(SERVO_DER_PIN);

  // Mover a la posición inicial
  moveServoX(90);
  moveRobot(1, 100);

  // 2. Configuración de la Cámara (Código original)
  camera_config_t config;
  // ... (tu código original de configuración de pines y resolución va aquí) ...
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);
    s->set_brightness(s, 1);
    s->set_saturation(s, -2);
  }
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);  // Reducir a QVGA para un stream inicial más rápido
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  // No necesitamos la función setupLedFlash() si usamos el GPIO 33 directamente
#endif

  // 3. Conexión WiFi
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // 4. Configurar el Manejador de Control antes de iniciar el servidor
  server.on("/control", HTTP_GET, handleControl);

  // 5. Iniciar Servidor de Cámara
  startCameraServer();

  Serial.print("Camera Ready! IP: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/stream");
}

void loop() {
  // El loop solo necesita atender las peticiones de control que no son stream
  server.handleClient();
  delay(1);  // Pequeño delay para no bloquear
}
