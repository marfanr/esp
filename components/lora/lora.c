#include "esp_log.h"                                                                                       
#include "lora.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "sdkconfig.h"

void lora_initialize() {
    // Initialize LoRa module
    ESP_LOGI("LORA", "Initializing LoRa module...");

    // meremap pin untuk rst dan cs
    gpio_config_t io_conf =  {
        .pin_bit_mask = (1ULL << CONFIG_LORA_RST_PIN) | (1ULL << CONFIG_LORA_CS_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf) == ESP_OK);
    gpio_set_direction(CONFIG_LORA_RST_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(CONFIG_LORA_CS_PIN, GPIO_MODE_OUTPUT);


}

void lora_register_write(int reg, int val) {
    
}