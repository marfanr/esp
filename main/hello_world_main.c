#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "soil7in1.h"
#include <inttypes.h>
#include <stdint.h>

void vTaskCode(void *pvParameters) {
  // while (true) {
  // struct request_data *data = (struct request_data *)pvParameters;
  soil_parameters_t soil_params;
  bulk_read_soil_parameters(&soil_params);
  ESP_LOGI(" Main", "Reading soil parameters...");
  ESP_LOGI("Main", "Soil Moisture: %" PRIu16, soil_params.soil_moist_value);
  ESP_LOGI("Main", "Soil Temperature: %" PRIu16, soil_params.soil_temp_value);
  ESP_LOGI("Main", "Soil Humidity: %" PRIu16, soil_params.soil_humidity_value);
  ESP_LOGI("Main", "Soil Conductivity: %" PRIu16, soil_params.soil_cond_value);
  ESP_LOGI("Main", "Soil Nitrogen: %" PRIu16, soil_params.soil_nitrogen_value);
  ESP_LOGI("Main", "Soil Phosphorus: %" PRIu16,
           soil_params.soil_phosporus_value);
  ESP_LOGI("Main", "Soil Potassium: %" PRIu16,
           soil_params.soil_potassium_value);
  // vTaskDelay(pdMS_TO_TICKS(1000));
  // }
}

void app_main(void) {
  // for 7in1 sensor
  // uart_setup();
  soil_initialize();

  // for rtc
  // i2c_setup();
  // while (true) {

  //   soil_parameters_t soil_params;
  //   bulk_read_soil_parameters(&soil_params);
  //   ESP_LOGI(" Main", "Reading soil parameters...");
  //   ESP_LOGI("Main", "Soil Moisture: %" PRIu16,
  //   soil_params.soil_moist_value); ESP_LOGI("Main", "Soil Temperature: %"
  //   PRIu16, soil_params.soil_temp_value); ESP_LOGI("Main", "Soil Humidity: %"
  //   PRIu16,
  //            soil_params.soil_humidity_value);
  //   ESP_LOGI("Main", "Soil Conductivity: %" PRIu16,
  //            soil_params.soil_cond_value);
  //   ESP_LOGI("Main", "Soil Nitrogen: %" PRIu16,
  //            soil_params.soil_nitrogen_value);
  //   ESP_LOGI("Main", "Soil Phosphorus: %" PRIu16,
  //            soil_params.soil_phosporus_value);
  //   ESP_LOGI("Main", "Soil Potassium: %" PRIu16,
  //            soil_params.soil_potassium_value);

  //   // sleep
  //   vTaskDelay(pdMS_TO_TICKS(1000));
  // }
  TaskHandle_t task_handle = NULL;
  xTaskCreatePinnedToCore(&vTaskCode, "read_sensor", 4096, NULL,  tskIDLE_PRIORITY, &task_handle, 0);
}
