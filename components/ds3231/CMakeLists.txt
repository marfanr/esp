idf_component_register(SRCS "ds3231.c"
                    PRIV_REQUIRES esp_driver_i2c
                    INCLUDE_DIRS "include")
