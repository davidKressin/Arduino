#include <SD.h>
#include <SPI.h>

// Pines del módulo SD para TTGO LoRa32 V2.0
#define SD_CS 4 // amarillo
#define SD_CLK 15 // gris
#define SD_MISO 2 // naranjo
#define SD_MOSI 13 // morado

File dataFile;

void setup() {
  Serial.begin(9600);
  // Configurar los pines del SPI para la tarjeta SD
  SPI.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);

  // Inicializar la tarjeta SD
  if (!SD.begin(SD_CS)) {
    Serial.println("Error al iniciar la tarjeta SD.");
    return;
  }

  Serial.println("Tarjeta SD inicializada.");
  
}

void loop() {
  dataFile = SD.open("/data.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("hola");
    dataFile.close();
    Serial.println("Escritura exitosa en data.txt");
  } else {
    Serial.println("Error al abrir data.txt para escribir.");
  }
  delay(5000);
}
