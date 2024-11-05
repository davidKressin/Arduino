#include <SPI.h>
#include <LoRa.h>

void setup() {
  // Iniciar puerto serial
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");

  LoRa.setPins(5, 14, 2);  // NSS, Reset, DIO0

  // Iniciar LoRa en frecuencia 433MHz
  if (!LoRa.begin(433E6)) {
    Serial.println("Error al iniciar LoRa");
    while (1);
  }
  Serial.println("LoRa iniciado correctamente");
}

void loop() {
  // Revisa si hay algún paquete LoRa recibido
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Leer el paquete LoRa
    Serial.print("Received packet ");

    //String receivedData = "";
    
    while (LoRa.available()) {
      //receivedData += (char)LoRa.read();
      Serial.print((char)LoRa.read());
    }

    // Mostrar los datos recibidos por LoRa en el puerto serial
    Serial.print("Datos recibidos por LoRa:");
    Serial.println(LoRa.packetRssi());
  }

  //delay(100); // Pausa para no saturar el serial
}
