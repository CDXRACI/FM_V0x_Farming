#include "fm_sht30_sensor.h"



static uint8_t FM_V0x_SHT30_Calc_CRC(const uint8_t *data, size_t length)
{
	uint8_t crc = 0xff;
	for (size_t i = 0; i < length; i++) {
		crc ^= data[i];
		for (size_t j = 0; j < 8; j++) {
			if ((crc & 0x80u) != 0) {
				crc = (uint8_t)((uint8_t)(crc << 1u) ^ 0x31u);
			} else {
				crc <<= 1u;
			}
		}
	}
	return crc;
}

static bool FM_V0x_SHT30_Send_Command(sht3x_handle_t *handle, sht3x_command_t command)
{
	uint8_t command_buffer[2] = {(command & 0xff00u) >> 8u, command & 0xffu};

	if (HAL_I2C_Master_Transmit(handle->i2c_handle, handle->device_address << 1u, command_buffer, sizeof(command_buffer),
	                            SHT3X_I2C_TIMEOUT) != HAL_OK) {
		return false;
	}

	return true;
}

static uint16_t FM_V0x_Conv_U8_2U16(uint8_t msb, uint8_t lsb)
{
	return (uint16_t)((uint16_t)msb << 8u) | lsb;
}


bool FM_V0x_SHT30_Sensor_Init(sht3x_handle_t *handle)
{
	assert(handle->i2c_handle->Init.NoStretchMode == I2C_NOSTRETCH_DISABLE);
	

	uint8_t status_reg_and_checksum[3];
	if (HAL_I2C_Mem_Read(handle->i2c_handle, handle->device_address << 1u, SHT3X_COMMAND_READ_STATUS, 2, (uint8_t*)&status_reg_and_checksum,
					  sizeof(status_reg_and_checksum), SHT3X_I2C_TIMEOUT) != HAL_OK) {
		return false;
	}

	uint8_t calculated_crc = FM_V0x_SHT30_Calc_CRC(status_reg_and_checksum, 2);

	if (calculated_crc != status_reg_and_checksum[2]) {
		return false;
	}

	return true;
}

bool FM_V0x_SHT30_Read_Temp_Humid(sht3x_handle_t *handle, float *temp, float *humid){
	 FM_V0x_SHT30_Send_Command(handle, SHT3X_COMMAND_MEASURE_HIGHREP_STRETCH);

	HAL_Delay(1);

	uint8_t buffer[6];
	if (HAL_I2C_Master_Receive(handle->i2c_handle, handle->device_address << 1u, buffer, sizeof(buffer), SHT3X_I2C_TIMEOUT) != HAL_OK) {
		return false;
	}

	uint8_t temp_crc = FM_V0x_SHT30_Calc_CRC(buffer, 2);
	uint8_t humid_crc = FM_V0x_SHT30_Calc_CRC(buffer + 3, 2);
	if (temp_crc != buffer[2] || humid_crc != buffer[5]) {
		return false;
	}

	uint16_t temp_raw = FM_V0x_Conv_U8_2U16(buffer[0], buffer[1]);
	uint16_t humid_raw = FM_V0x_Conv_U8_2U16(buffer[3], buffer[4]);

	*temp = -45.0f + 175.0f * (float)temp_raw / 65535.0f;
	*humid = 100.0f * (float)humid_raw / 65535.0f;

	return true;
}

bool FM_V0x_SHT30_Set_Header_Enable(sht3x_handle_t *handle, bool enable)
{
	if (enable) {
		return FM_V0x_SHT30_Send_Command(handle, SHT3X_COMMAND_HEATER_ENABLE);
	} else {
		return FM_V0x_SHT30_Send_Command(handle, SHT3X_COMMAND_HEATER_DISABLE);
	}
}

