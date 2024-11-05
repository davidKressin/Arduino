#include <SPI.h>
#include <LoRa.h>

// Definir el pin del sensor
const int sensorPin = A13;
const int humedadAire = 4100;
const int humedadAgua = 300;
int porcentajeHumedad; // Declarar la variable porcentajeHumedad

// Variable para controlar el envío de datos
bool midiendo = true;

// Variable para el temporizador
unsigned long previousMillis = 0;
const long interval = 5000; // Intervalo de 5 segundos

void setup() {
  Serial.begin(115200);
  Serial.println("Holaa");


  LoRa.setPins(5, 14, 2);  // NSS, Reset, DIO0
  // Iniciar LoRa en frecuencia 433MHz
  if (!LoRa.begin(433E6)) {
    Serial.println("Error al iniciar LoRa");
    while (1);
  }
  
  Serial.println("LoRa iniciado correctamente en 433MHz");
}

void loop() {
  unsigned long currentMillis = millis();

  if (midiendo) {
    // Leer la humedad del sensor
    int humedad = analogRead(sensorPin);
    porcentajeHumedad = map(humedad, humedadAire, humedadAgua, 0, 100);
    if (porcentajeHumedad > 100) porcentajeHumedad = 100;

    Serial.print("Humedad: ");
    Serial.print(humedad);
    Serial.print("-");
    Serial.print(porcentajeHumedad);
    Serial.println("%");

    // Enviar datos a través de LoRa cada 5 segundos
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      enviarDatosLoRa(humedad, porcentajeHumedad);
    }
  }

  delay(10000); // Pausa para no saturar el serial
}

void enviarDatosLoRa(int humedad, int porcentajeHumedad) {
  // Crear un string de datos para enviar
  String datos = "Humedad: " + String(humedad) + " Porcentaje: " + String(porcentajeHumedad) + "%";

  // Iniciar la transmisión LoRa
  LoRa.beginPacket();
  LoRa.print(datos);
  LoRa.endPacket();

  Serial.println("Datos enviados por LoRa: " + datos);
}
