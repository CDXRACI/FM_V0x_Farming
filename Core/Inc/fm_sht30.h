/****************************************************************************************************
 * @file :        - fm_sht30.h
 * --------------------------------------------------------------------------------------------------
 * @brief:        - Declare parameters, functions that will be used in sht30.
 *                - Purpose, Get data will return from sht30 sensor.
 * @version  :    - 0.0.1
 * --------------------------------------------------------------------------------------------------
 * @author   :    - CDP_Darkness
 * @copyright:    - DP.Greens © all rights reserved          
 * @date     :    - 2024-10-10
 * *************************************************************************************************/

#ifndef     __FM_SHT30_H
#define     __FM_SHT30_H

#ifdef      __cplusplus
extern      "C" {
#endif    

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdbool.h> 
#include <assert.h>

#ifndef SHT3X_I2C_TIMEOUT
#define SHT3X_I2C_TIMEOUT 30
#endif

#define SHT3X_I2C_DEVICE_ADDRESS_ADDR_PIN_LOW  0x44
#define SHT3X_I2C_DEVICE_ADDRESS_ADDR_PIN_HIGH 0x45


typedef struct {

	I2C_HandleTypeDef *i2c_handle;
	uint16_t device_address;

} sht3x_handle_t;

typedef enum
{
	SHT3X_COMMAND_MEASURE_HIGHREP_STRETCH       = 0x2C06,
	SHT3X_COMMAND_CLEAR_STATUS                  = 0x3041,
	SHT3X_COMMAND_SOFT_RESET                    = 0x30A2,
	SHT3X_COMMAND_HEATER_ENABLE                 = 0x306D,
	SHT3X_COMMAND_HEATER_DISABLE                = 0x3066,
	SHT3X_COMMAND_READ_STATUS                   = 0xF32D,
	SHT3X_COMMAND_FETCH_DATA                    = 0xe000,
	SHT3X_COMMAND_MEASURE_HIGHREP_10HZ          = 0x2737,
	SHT3X_COMMAND_MEASURE_LOWREP_10HZ           = 0x272A
} sht3x_command_t;

bool            FM_V0x_SHT30_Sensor_Init(sht3x_handle_t *handle);
bool            FM_V0x_SHT30_Read_Temp_Humid(sht3x_handle_t *handle, float *temp, float *humid);
bool            FM_V0x_SHT30_Set_Header_Enable(sht3x_handle_t *handle, bool enable);
static uint8_t  FM_V0x_SHT30_Calc_CRC(const uint8_t *data, size_t length);
static bool     FM_V0x_SHT30_Send_Command(sht3x_handle_t *handle, sht3x_command_t command);
static uint16_t FM_V0x_Conv_U8_2U16(uint8_t msb, uint8_t lsb);


#ifdef      __cplusplus
}
#endif
#endif