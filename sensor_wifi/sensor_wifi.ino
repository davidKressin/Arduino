#include <ArduinoJson.h>            // https://github.com/bblanchon/ArduinoJson
#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>
#include <WifiUdp.h>
#include <NTPClient.h>


#define _SSID "udd-recicla"          // Tu SSID de WiFi
#define _PASSWORD "R3c1claj3#udd-2019"             // Tu contraseña de WiFi
#define REFERENCE_URL "https://sensorestid-default-rtdb.firebaseio.com/"  // URL de tu proyecto en Firebase

WiFiUDP udp;                            // Instancia de WiFiUDP para comunicación UDP
Firebase firebase(REFERENCE_URL);
NTPClient timeClient(udp, "pool.ntp.org", 3600); // Offset para zona horaria

// Definir el pin del sensor
const int sensorPin = A0;
const int humedadAire = 700;
const int humedadAgua = 300;
int porcentajeHumedad; // Declarar la variable porcentajeHumedad

// Variable para controlar el envío de datos
bool midiendo = true;

// Variable para el temporizador
unsigned long previousMillis = 0;
const long interval = 5000; // Intervalo de 2 minutos (120000 ms)

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Conectar a WiFi
  Serial.println();
  Serial.print("Conectando a: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Conectado");

  // Iniciar cliente NTP
  timeClient.begin();
  timeClient.update();

  // Imprimir la dirección IP
  Serial.print("Dirección IP: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  digitalWrite(LED_BUILTIN, HIGH);


  // Configura la hora inicial
  timeClient.update();

}

void loop() {
  // Actualiza el tiempo
  timeClient.update();
  unsigned long timestamp = timeClient.getEpochTime(); // Timestamp en segundos desde 1970

  unsigned long currentMillis = millis();

  if (midiendo) {
    // Leer la humedad del sensor
    
    String porcentajeRuta = "don_pancho/registros/todos/" + String(timestamp) + "/sensor_3/porcentaje"; // Suponiendo que el sensorID es 1
    String nivelRuta = "don_pancho/registros/todos/" + String(timestamp) + "/sensor_3/nivel"; // Suponiendo que el sensorID es 1
    String ultimoRegistroRuta = "don_pancho/registros/ultimo/sensor_3/porcentaje"; // Suponiendo que el sensorID es 1
    int humedad = analogRead(sensorPin);
    porcentajeHumedad = map(humedad, humedadAire, humedadAgua, 0, 100);
    if (porcentajeHumedad > 100) porcentajeHumedad = 100;

    Serial.print("Humedad: ");
    Serial.print(humedad);
    Serial.print("-");
    Serial.print(porcentajeHumedad);
    Serial.println("%");

    // Enviar datos a Firebase cada 2 minutos
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // Enviar datos a Firebase
      if (firebase.setFloat(nivelRuta, humedad)) {
        Serial.println("Datos enviados a Firebase correctamente.");
      } else {
        Serial.print("Error al enviar datos: ");
      }

      if (firebase.setFloat(porcentajeRuta, porcentajeHumedad)) {
        Serial.println("Datos enviados a Firebase correctamente.");
      } else {
        Serial.print("Error al enviar datos: ");
      }

      if (firebase.setFloat(ultimoRegistroRuta, porcentajeHumedad)) {
        Serial.println("Datos enviados a Firebase correctamente.");
      } else {
        Serial.print("Error al enviar datos: ");
      }

    }
  }

  delay(1000); // Pequeña pausa para no saturar el serial
}