#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);  // CE, CSN

int receivedValue = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, 0xF0F0F0F0E1LL); // Dirección del pipe (debe coincidir con el transmisor)
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&receivedValue, sizeof(receivedValue));
    Serial.print("Received Value: ");
    Serial.println(receivedValue);
  }
  delay(100);
}
