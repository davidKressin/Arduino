#include <Wire.h>
#include <SSD1306Wire.h>  // Biblioteca OLED para ESP32 y ESP8266
#include <SPI.h>
#include <LoRa.h>  // Biblioteca para LoRa

// Definir pines para LoRa TTGO ESP32 LoRa v2
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DI0 26
#define BAND 915E6  // Frecuencia (ajustar según la región)

// Definir pines para OLED
#define OLED_SDA 21
#define OLED_SCL 22

// Inicializar la pantalla OLED
SSD1306Wire display(0x3c, OLED_SDA, OLED_SCL);

void setup() {
  // Inicialización de Serial para monitoreo
  Serial.begin(9600);

  // Inicializar OLED
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.drawString(0, 0, "Inicializando...");
  display.display();

  // Configurar pines de LoRa
  LoRa.setPins(SS, RST, DI0);

  // Inicializar LoRa a la frecuencia configurada
  if (!LoRa.begin(BAND)) {
    Serial.println("No se pudo iniciar LoRa.");
    display.clear();
    display.drawString(0, 20, "Error LoRa");
    display.display();
    while (1)
      ;
  }

  // Mostrar que LoRa ha sido inicializado
  Serial.println("LoRa inicializado.");
  display.clear();
  display.drawString(0, 20, "LoRa Iniciado");
  display.display();
  delay(1000);
}

void loop() {
  // Verificar si se ha recibido algún paquete LoRa
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    // Mostrar que un paquete ha sido recibido
    Serial.print("Paquete recibido: ");
    Serial.println(packetSize);

    // Leer el contenido del paquete LoRa
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }

    // Mostrar el contenido recibido por Serial
    Serial.print("Contenido: ");
    Serial.println(received);

    // Mostrar en OLED el contenido recibido
    display.clear();
    display.drawString(0, 0, "Sensor 1:");
    display.drawString(0, 20, received);
    display.drawString(0, 40, "RSSI: " + String(LoRa.packetRssi()));
    display.display();

    // Mostrar la RSSI (Received Signal Strength Indicator)
    Serial.print("Señal RSSI: ");
    Serial.println(LoRa.packetRssi());
  }
}