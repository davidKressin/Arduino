#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);  // CE, CSN

const int sensorPin = A0; // Pin del sensor capacitivo de humedad

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(0xF0F0F0F0E1LL); // Dirección del pipe
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  radio.write(&sensorValue, sizeof(sensorValue));
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  delay(1000); // Ajusta el intervalo de tiempo según tus necesidades
}
