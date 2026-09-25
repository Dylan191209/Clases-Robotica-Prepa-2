#include <LiquidCrystal.h>

// Definición de pines para la LCD según tu esquema:
// rs (Register Select) = Pin 12
// e (Enable) = Pin 11
// d4 (Data Pin 4) = Pin 5
// d5 (Data Pin 5) = Pin 4
// d6 (Data Pin 6) = Pin 3
// d7 (Data Pin 7) = Pin 2
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // ¡Estos son los pines CORRECTOS para tu configuración!

char incomingChar = 0; // Variable para almacenar el carácter recibido

void setup() {
  Serial.begin(19200); // ¡Comunicación con la Maestra a 19200 baudios!

  lcd.begin(16, 2); // Inicializa el LCD con 16 columnas y 2 filas

  // Este delay da tiempo al LCD para inicializarse completamente
  delay(1000); // Espera 1 segundo (1000 milisegundos)

  // Mensaje inicial que se mostrará por defecto, también cuando esté "AVANZANDO"
  lcd.print("Robot Iniciado"); // Mensaje inicial
  lcd.setCursor(0, 1);
  lcd.print("Esperando...");
}

void loop() {
  // Siempre que no haya un nuevo comando, el mensaje "Polaris v3" permanecerá.
  // Solo actualizamos el LCD si hay un nuevo carácter serial disponible.
  if (Serial.available() > 0) {
    incomingChar = Serial.read(); // Lee el carácter entrante

    // Solo limpia la pantalla si el comando NO es "AVANZAR"
    // para evitar el parpadeo constante del mensaje de inicio
    if (incomingChar != 'A') {
      lcd.clear();
    }

    switch (incomingChar) {
      case 'A': // Avanzar
        // Si el robot avanza, no hacemos nada más que asegurar el mensaje inicial
        // ya que este es el estado por defecto.
        // Si el LCD ya mostraba "Polaris v3", no lo redibujamos.
        // Para evitar parpadeo si venía de otro estado, podrías forzarlo así:
        // lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Polaris v3");
        lcd.setCursor(0, 1);
        lcd.print("Prepa2 robotica");
        // No hay delay aquí porque el robot podría pasar mucho tiempo avanzando
        // y este es el estado "por defecto" en la pantalla.
        break;
      case 'R': // Retroceder
        lcd.setCursor(0, 0);
        lcd.print("RETROCEDIENDO");
        delay(500); // Mantiene el mensaje 0.5 segundos para que se lea
        break;
      case 'D': // Derecha (giro único)
        lcd.setCursor(0, 0);
        lcd.print("GIRO DERECHA");
        delay(500);
        break;
      case 'I': // Izquierda (giro único)
        lcd.setCursor(0, 0);
        lcd.print("GIRO IZQUIERDA");
        delay(500);
        break;
      case 'P': // Parar
        lcd.setCursor(0, 0);
        lcd.print("DETENIDO");
        delay(1000); // Mantiene el mensaje de detener más tiempo
        break;
      case 'U': // Giro 180 (U-turn)
        lcd.setCursor(0, 0);
        lcd.print("GIRO 180 GRADOS");
        delay(1000);
        break;
      case 'd': // Derecha2 (minúscula: más espacio a derecha)
        lcd.setCursor(0, 0);
        lcd.print("ESPACIO DER.");
        lcd.setCursor(0, 1);
        lcd.print("Giro Derecha");
        delay(750); // Un poco más de tiempo para mensajes de 2 líneas
        break;
      case 'i': // Izquierda2 (minúscula: más espacio a izquierda)
        lcd.setCursor(0, 0);
        lcd.print("ESPACIO IZQ.");
        lcd.setCursor(0, 1);
        lcd.print("Giro Izquierda");
        delay(750); // Un poco más de tiempo para mensajes de 2 líneas
        break;
      default:
        lcd.setCursor(0, 0);
        lcd.print("CMD DESCONOCIDO");
        delay(1000);
        break;
    }
  }
}