#include "soil7in1.h"
#include <stdint.h>
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define DE_RE 4

uint16_t crc16(uint8_t *data, int length) {
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

void soil_initialize() {
    ESP_LOGI("Soil7in1", "Initializing soil sensor");
    
    // Set up UART for communication
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    QueueHandle_t queue;
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, 4096, 4096, 20, &queue,0));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, CONFIG_SENSOR_TX_PIN, CONFIG_SENSOR_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));
    gpio_reset_pin(DE_RE);
    gpio_set_direction(DE_RE, GPIO_MODE_OUTPUT);
}

void bulk_read_soil_parameters(soil_parameters_t *soil_params) {
    inquirity_frame_t frame;
    frame.address = 0x01;
    frame.func = 0x03;
    frame.start = SOIL_MOIST_VALUE_REG;
    frame.length = 0x0007; // baca 7 parameters
    frame.CRC = crc16((uint8_t *)&frame, sizeof(frame) - sizeof(frame.CRC));
    gpio_set_level(DE_RE, 1); // Set DE/RE pin high to enable transmission
    uart_write_bytes(UART_NUM_2, (const char *)&frame, sizeof(frame));
    // int a = uart_write_bytes(UART_NUM_2, "hallo", 5);
//    ESP_LOGI("Soil7in1", "Sending request to soil sensor, bytes sent: %d", a);
    uart_wait_tx_done(UART_NUM_2, pdMS_TO_TICKS(100));


    gpio_set_level(DE_RE, 0); // Set DE/RE pin low to enable reception
    uint8_t response[21] = {0}; // metadata + 7 parameters * 2 byte
    int length = uart_read_bytes(UART_NUM_2, response, sizeof(response) - 1,
                                 pdMS_TO_TICKS(1000));
    ESP_LOGI("Soil7in1", "Received response length: %d", length);

    if (length < 21) {
        ESP_LOGW("Soil7in1", "Failed to read all parameters");
        return;
    }

    soil_params->soil_moist_value = (response[3] << 8) | response[4];
    soil_params->soil_temp_value = (response[5] << 8) | response[6];
    soil_params->soil_humidity_value = (response[7] << 8) | response[8];
    soil_params->soil_cond_value = (response[9] << 8) | response[10];
    soil_params->soil_nitrogen_value = (response[11] << 8) | response[12];
    soil_params->soil_phosporus_value = (response[13] << 8) | response[14];
    soil_params->soil_potassium_value = (response[15] << 8) | response[16];
}
