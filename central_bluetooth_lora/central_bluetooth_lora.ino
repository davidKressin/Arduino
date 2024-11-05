#include <SPI.h>
#include <LoRa.h>
#include "BluetoothSerial.h"  // Librería para Bluetooth en ESP32

// Inicializa Bluetooth
BluetoothSerial SerialBT;

void setup() {
  // Iniciar puerto serial
  Serial.begin(115200);
  Serial.println("hola");

  // Iniciar LoRa en frecuencia 433MHz
  if (!LoRa.begin(433E6)) {
    Serial.println("Error al iniciar LoRa");
    while (1);
  }
  Serial.println("LoRa iniciado correctamente");

  // Iniciar Bluetooth con nombre "ESP32_Bluetooth"
  if (!SerialBT.begin("ESP32_Bluetooth")) {
    Serial.println("Error al iniciar Bluetooth");
    return;
  }
  Serial.println("Bluetooth inicializado. Esperando conexión...");
}

void loop() {
  // Revisa si hay algún paquete LoRa recibido
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Leer el paquete LoRa
    String receivedData = "";
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    Serial.println("Datos recibidos por LoRa:");
    Serial.println(receivedData);

    // Verificar si hay un cliente Bluetooth conectado
    if (SerialBT.hasClient()) {
      // Enviar los datos recibidos por LoRa a través de Bluetooth
      SerialBT.println("Datos recibidos por LoRa, enviando vía Bluetooth:");
      SerialBT.println(receivedData);
    } else {
      Serial.println("Esperando conexión Bluetooth...");
    }
  }

  delay(1000); // Pausa para no saturar el serial
}
