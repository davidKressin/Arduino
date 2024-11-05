#include <SPI.h>
#include <LoRa.h>

// Definir el pin del sensor
const int sensorPin = A0;

// Calibración sensor
const int humedadAire = 448;
const int humedadAgua = 219;
int porcentajeHumedad; // Declarar la variable porcentajeHumedad


void setup() {
  Serial.begin(115200);
  Serial.println("Holaa");


  LoRa.setPins(10, 9, 2);  // NSS, Reset, DIO0
  // Iniciar LoRa en frecuencia 433MHz
  if (!LoRa.begin(433E6)) {
    Serial.println("Error al iniciar LoRa");
    while (1);
  }
  
}

void loop() {

  // Leer la humedad del sensor
  int humedad = analogRead(sensorPin);
  porcentajeHumedad = map(humedad, humedadAire, humedadAgua, 0, 100);
  if (porcentajeHumedad > 100) porcentajeHumedad = 100;

  //String datos = "Humedad: " + String(humedad) + " Porcentaje: " + String(porcentajeHumedad) + "% ";
  
  Serial.print("enviando...");

  LoRa.beginPacket();
  LoRa.print(porcentajeHumedad);
  LoRa.endPacket();

  Serial.print("Datos enviados por LoRa: "+ porcentajeHumedad);
  Serial.println("");
  delay(2000); // Pausa para no saturar el serial
}


