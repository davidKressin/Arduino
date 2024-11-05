#include <Wire.h>
#include <SSD1306Wire.h>  // Biblioteca OLED para ESP32 y ESP8266
#include <SPI.h>
#include <LoRa.h>  // Biblioteca para LoRa
#include <SD.h>  // Biblioteca para SD

// Pines del módulo SD para TTGO LoRa32 V2.0
#define SD_CS 4 // amarillo
#define SD_CLK 15 // gris
// #define SD_MISO 2 // naranjo
#define SD_MISO 19 // naranjo
#define SD_MOSI 13 // morado

// Pines para LoRa (con VSPI)
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DI0 26
#define BAND 915E6  // Frecuencia (ajustar según la región)

// Pines para OLED
#define OLED_SDA 21
#define OLED_SCL 22

// Inicializar la pantalla OLED
SSD1306Wire display(0x3c, OLED_SDA, OLED_SCL);

// Crear instancias SPI para LoRa y SD
SPIClass *vspi = NULL;  // VSPI para LoRa
SPIClass *hspi = NULL;  // HSPI para SD

void setup() {
  // Inicialización de Serial para monitoreo
  Serial.begin(9600);

  // Inicializar OLED
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.drawString(0, 0, "Inicializando...");
  display.display();

  // Configurar e inicializar LoRa en VSPI
  vspi = new SPIClass(VSPI);  
  vspi->begin(SCK, MISO, MOSI, SS);
  LoRa.setSPI(*vspi);
  LoRa.setPins(SS, RST, DI0);

  delay(1000);  // Retardo para estabilizar

  if (!LoRa.begin(BAND)) {
    Serial.println("No se pudo iniciar LoRa.");
    display.clear();
    display.drawString(0, 40, "Error LoRa");
    display.display();
    while (1);
  }

  // // Mostrar que LoRa ha sido inicializado
  Serial.println("LoRa inicializado.");
  display.clear();
  display.drawString(0, 40, "LoRa Iniciado");
  display.display();
  delay(1000);

  // Ahora inicializa la tarjeta SD después de LoRa
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  
  if (!SD.begin(SD_CS)) {
    Serial.println("Error al iniciar la tarjeta SD.");
    display.clear();
    display.drawString(0, 20, "Error SD");
    display.display();
    while (1);
  }

  Serial.println("Tarjeta SD inicializada.");
  display.drawString(0, 20, "SD Iniciada");
  display.display();
  delay(1000);
}

void loop() {
  // Verificar si se ha recibido algún paquete LoRa
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Leer el contenido del paquete LoRa
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }

    int rssi = LoRa.packetRssi(); // Leer la intensidad de la señal RSSI

    // Mostrar el contenido recibido por Serial
    Serial.print("Contenido: ");
    Serial.println(received);
    Serial.print("Señal RSSI: ");
    Serial.println(rssi);

    // Mostrar en OLED el contenido recibido
    display.clear();
    display.drawString(0, 0, "Sensor 1:");
    display.drawString(0, 20, received);
    display.drawString(0, 40, "RSSI: " + String(rssi));
    display.display();

    // Guardar los datos en la tarjeta SD
    File dataFile = SD.open("/datos.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print("Datos: ");
      dataFile.print(received);
      dataFile.print(" | RSSI: ");
      dataFile.println(rssi);
      dataFile.close();
      Serial.println("Datos guardados en SD.");
    } else {
      Serial.println("Error al abrir datos.txt para escribir.");
    }
  }
}
