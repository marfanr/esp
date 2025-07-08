#include "lora.h"
#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "hal/spi_types.h"
#include "sdkconfig.h"
// #include <cstddef>

void setup_spi_device() {
  spi_bus_config_t buscfg = {
      .miso_io_num = CONFIG_LORA_MISO_PIN,
      .mosi_io_num = CONFIG_LORA_MOSI_PIN,
      .sclk_io_num = CONFIG_LORA_SCLK_PIN,
      // gk butuh quad spi (-1 buat disabled)
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 0, // default
  };
  // makek spi host 1 + gk butuh dma (direct memory access)
  ESP_ERROR_CHECK(spi_bus_initialize(SPI1_HOST, &buscfg, SPI_DMA_DISABLED));

                  //   register spi device
  spi_device_interface_config_t devcfg = {
      .clock_speed_hz = CONFIG_LORA_SPI_CLOCK_HZ, 
      .mode = 0,                                  // polarity 0, phase 0
      .spics_io_num = -1,
      .flags = 0,
      .queue_size = 1,      
  };
  ESP_ERROR_CHECK(spi_bus_add_device(SPI1_HOST, &devcfg, NULL));

}

void lora_initialize() {
  // Initialize LoRa module
  ESP_LOGI("LORA", "Initializing LoRa module...");

  // meremap pin untuk rst dan cs
  gpio_config_t io_conf = {
      .pin_bit_mask =
          (1ULL << CONFIG_LORA_RST_PIN) | (1ULL << CONFIG_LORA_CS_PIN),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };
  ESP_ERROR_CHECK(gpio_config(&io_conf) == ESP_OK);
  gpio_set_direction(CONFIG_LORA_RST_PIN, GPIO_MODE_OUTPUT);
  gpio_set_direction(CONFIG_LORA_CS_PIN, GPIO_MODE_OUTPUT);

  setup_spi_device();
}

void lora_register_write(int reg, int val) {
    uint8_t s[2] = {0x80 | reg, val};
    uint8_t i[2] = {0};
    spi_transaction_t t = {
        .flags = 0,
        .length = 8 * sizeof(s),
        .tx_buffer = s,
        .rx_buffer = i,
    };
    gpio_set_level(CONFIG_LORA_CS_PIN, 0); // CS low
    ESP_ERROR_CHECK(spi_device_transmit(SPI1_HOST, &t));
    gpio_set_level(CONFIG_LORA_CS_PIN, 1); // CS high
}

int lora_register_read(int reg) {
    uint8_t s[2] = {reg, 0x00};
    uint8_t i[2] = {0};
    spi_transaction_t t = {
        .flags = 0,
        .length = 8 * sizeof(s),
        .tx_buffer = s,
        .rx_buffer = i,
    };
    gpio_set_level(CONFIG_LORA_CS_PIN, 0); // CS low
    ESP_ERROR_CHECK(spi_device_transmit(SPI1_HOST, &t));
    gpio_set_level(CONFIG_LORA_CS_PIN, 1); // CS high
    return i[1];
}