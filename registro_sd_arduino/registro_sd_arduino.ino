#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;  // Pin CS para el módulo SD

void setup() {
  // Iniciar comunicación serial
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Iniciando Arduino UNO...");

  // Inicializar la tarjeta SD
  if (!SD.begin(chipSelect)) {
    Serial.println("Error al inicializar la tarjeta SD");
    while (1);
  }
  Serial.println("Tarjeta SD inicializada.");
}

void loop() {
  // Verificar si hay datos seriales disponibles desde el TTGO
  if (Serial.available()) {
    String received = Serial.readStringUntil('\n');  // Leer hasta el final de la línea
    received.trim();  // Eliminar espacios en blanco

    // Mostrar el mensaje recibido en el monitor serial
    Serial.print("Mensaje recibido: ");
    Serial.println(received);

    // Guardar el mensaje en la tarjeta SD
    File dataFile = SD.open("datos.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(received);
      dataFile.close();
      Serial.println("Datos guardados en SD");
    } else {
      Serial.println("Error al escribir en SD");
    }
  }

  delay(1000);  // Pausa para evitar saturación de lectura
}
