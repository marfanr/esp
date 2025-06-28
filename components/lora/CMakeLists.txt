idf_component_register(SRCS "lora.c"                    
                    PRIV_REQUIRES esp_driver_gpio esp_driver_spi
                    INCLUDE_DIRS "include")
