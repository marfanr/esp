#ifndef __SOIL7IN1_H__
#define __SOIL7IN1_H__

#include <stdint.h>

typedef struct {
  uint8_t address;
  uint8_t func;
  uint16_t start;
  uint16_t length;
  uint16_t CRC;
} inquirity_frame_t;

typedef struct {
  uint8_t address;
  uint8_t func;
  uint8_t effective_number;  
} response_frame_meadata_t;

#define PH_VALUE_REG 0x6
#define SOIL_MOIST_VALUE_REG 0x12
#define SOIL_TEMP_REG 0x13
#define SOIL_COND_REG 0x15
#define SOIL_NITROGEN_REG 0x1E
#define SOIL_PHOSPORUS_REG 0x1F
#define SOIL_POTASIUM_REG 0x20
#define EQUIP_ADDR_REG 0x100
#define BAU_RATE_REG 0x101

typedef struct {    
    uint16_t soil_moist_value;
    uint16_t soil_temp_value;
    uint16_t soil_humidity_value;
    uint16_t soil_cond_value;
    uint16_t soil_nitrogen_value;
    uint16_t soil_phosporus_value;
    uint16_t soil_potassium_value;
} soil_parameters_t;


void soil_initialize();
void bulk_read_soil_parameters(soil_parameters_t *soil_params);

#endif