#include "BluetoothSerial.h"  // Importa la biblioteca BluetoothSerial

// Inicializa Bluetooth para ESP32
BluetoothSerial SerialBT;  // Sin argumentos

// Definición del JSON que contiene las zonas y sus niveles de humedad
const char* zonas = R"(
[
  {"zona": "Zona 1", "humedad": 60},
  {"zona": "Zona 2", "humedad": 80},
  {"zona": "Zona 3", "humedad": 50},
  {"zona": "Zona 4", "humedad": 30},
  {"zona": "Zona 5", "humedad": 90}
]
)";

void setup() {
  // Inicia el puerto serial
  Serial.begin(115200);
  
  // Inicia el Bluetooth con el nombre "ESP32_Bluetooth"
  if (!SerialBT.begin("ESP32_Bluetooth")) {  // Esto debería funcionar en ESP32
    Serial.println("Error al inicializar Bluetooth");
    return;
  }
  
  Serial.println("Bluetooth inicializado. Esperando conexión...");
}

void loop() {
  // Verifica si hay una conexión Bluetooth activa
  if (SerialBT.hasClient()) {  // hasClient es válido para ESP32
    // Envía los datos de zonas al dispositivo conectado vía Bluetooth
    SerialBT.println("Enviando datos de humedad de zonas...");
    enviarDatos();
    delay(10000); // Espera 10 segundos antes de enviar nuevamente
  } else {
    Serial.println("Esperando conexión Bluetooth...");
  }

  delay(1000); // Revisa cada segundo si hay conexión
}

void enviarDatos() {
  // Enviar el JSON directamente como texto
  SerialBT.print("Archivo 'data':\n");
  SerialBT.print(zonas);
  Serial.println("Datos enviados vía Bluetooth.");
}
