#include <SPI.h>
#include <LoRa.h>

const int csPin = 18;          // Chip select (CS) pin for LoRa module
const int resetPin = 14;       // Reset pin for LoRa module
const int irqPin = 26;         // IRQ pin for LoRa module

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Transmitter");

  // Define the pins for LoRa communication
  LoRa.setPins(csPin, resetPin, irqPin);

  // Initialize LoRa at 915 MHz (adjust frequency if needed)
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.println("Sending packet...");

  // Start the packet
  LoRa.beginPacket();
  
  // Send a message, you can change the content here
  LoRa.print("Hello from TTGO LoRa");
  
  // End the packet and send it
  LoRa.endPacket();

  // Wait for a few seconds before sending the next packet
  delay(5000); // send every 5 seconds
}
