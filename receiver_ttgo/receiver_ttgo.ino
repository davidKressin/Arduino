#include <SPI.h>
#include <LoRa.h>

const int csPin = 18;          // Chip select (CS) pin for LoRa module
const int resetPin = 14;       // Reset pin for LoRa module
const int irqPin = 26;         // IRQ pin for LoRa module

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");

  // Define the pins for LoRa communication
  LoRa.setPins(csPin, resetPin, irqPin);

  // Initialize LoRa at 915 MHz (adjust frequency if needed)
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // Check if there's an incoming packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // If a packet is received, print the content
    Serial.print("Received packet: ");

    while (LoRa.available()) {
      String receivedData = LoRa.readString();
      Serial.print(receivedData);
    }
    Serial.println();
    
    // Print RSSI (signal strength)
    Serial.print("Signal strength: ");
    Serial.println(LoRa.packetRssi());
  }
}
