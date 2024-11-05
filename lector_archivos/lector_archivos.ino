#include <SPIFFS.h>
#include "esp_task_wdt.h" // Incluye la biblioteca del WDT

void setup() {
  Serial.begin(115200);

  // Configuración del WDT con un tiempo de espera de 50 segundos
  esp_task_wdt_config_t wdt_config = {
      .timeout_ms = 50000, // 50 segundos
      .idle_core_mask = ESP_TASK_WDT_IDLE_CORE_0 | ESP_TASK_WDT_IDLE_CORE_1, // Aplica en ambos núcleos
      .trigger_panic = true // Activa un 'panic' en caso de falla
  };
  esp_task_wdt_init(&wdt_config);

  // Inicia SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar SPIFFS");
    return;
  }
}

void loop() {
  // Aquí puedes realizar tareas periódicas si es necesario
}
