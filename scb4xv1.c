/*
 * scb4xv1.c
 *
 *  Created on: Dec 22, 2025
 *      Author: leyva
 */


#include "scb4xv1.h"

static I2C_HandleTypeDef *hi2c_scb4xv1;
static TIM_HandleTypeDef *htim_scb4xv1;

void scb4xv1_init(I2C_HandleTypeDef *hi2c, TIM_HandleTypeDef *htim){
	hi2c_scb4xv1 = hi2c;
	htim_scb4xv1 = htim;
}

void wait_ms(uint16_t ms){
	/*
	 * This function IS cpu blocking. Consider using a non-blocking alternative
	 * when using RTOS.
	 */

	__HAL_TIM_SET_COUNTER(htim_scb4xv1, 0);
	HAL_TIM_Base_Start(htim_scb4xv1);
	while (__HAL_TIM_GET_COUNTER(htim_scb4xv1) < ms);
	HAL_TIM_Base_Stop(htim_scb4xv1);
}

/* SHT4X functions*/
uint8_t Temp_RH_readRaw_HighPrecision(uint16_t *dataArray){
	uint8_t cmd = MEASURE_HIGH_PRECISION_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	wait_ms(9);

	uint8_t data[6];
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint8_t dataTemp[2];
	uint8_t dataRH[2];
	dataTemp[0] = data[0];
	dataTemp[1] = data[1];
	dataRH[0] = data[3];
	dataRH[1] = data[4];

	uint8_t temp_CRC = data[2];
	uint8_t rh_CRC = data[5];

	uint8_t tempDataValid = check_CRC(dataTemp, 2, temp_CRC);
	uint8_t rhDataValid = check_CRC(dataRH, 2, rh_CRC);

	if (tempDataValid && rhDataValid){
		uint16_t rawTemp = data[0] * 256 + data[1];
		uint16_t rawRH = data[3] * 256 + data[4];
		
		dataArray[0] = rawTemp;
		dataArray[1] = rawRH;
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t Temp_RH_readRaw_MediumPrecision(uint16_t *dataArray){
	uint8_t cmd = MEASURE_MEDIUM_PRECISION_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	wait_ms(5);

	uint8_t data[6];
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint8_t dataTemp[2];
	uint8_t dataRH[2];
	dataTemp[0] = data[0];
	dataTemp[1] = data[1];
	dataRH[0] = data[3];
	dataRH[1] = data[4];

	uint8_t temp_CRC = data[2];
	uint8_t rh_CRC = data[5];

	uint8_t tempDataValid = check_CRC(dataTemp, 2, temp_CRC);
	uint8_t rhDataValid = check_CRC(dataRH, 2, rh_CRC);

	if (tempDataValid && rhDataValid){
		uint16_t rawTemp = data[0] * 256 + data[1];
		uint16_t rawRH = data[3] * 256 + data[4];
		dataArray[0] = rawTemp;
		dataArray[1] = rawRH;
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t Temp_RH_readRaw_LowPrecision(uint16_t *dataArray){
	uint8_t cmd = MEASURE_LOW_PRECISION_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	wait_ms(2);

	uint8_t data[6];
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint8_t dataTemp[2];
	uint8_t dataRH[2];
	dataTemp[0] = data[0];
	dataTemp[1] = data[1];
	dataRH[0] = data[3];
	dataRH[1] = data[4];

	uint8_t temp_CRC = data[2];
	uint8_t rh_CRC = data[5];

	uint8_t tempDataValid = check_CRC(dataTemp, 2, temp_CRC);
	uint8_t rhDataValid = check_CRC(dataRH, 2, rh_CRC);

	if (tempDataValid && rhDataValid){
		uint16_t rawTemp = data[0] * 256 + data[1];
		uint16_t rawRH = data[3] * 256 + data[4];
		dataArray[0] = rawTemp;
		dataArray[1] = rawRH;
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t Temp_RH_read_HighPrecision(float *dataArray){
	uint8_t cmd = MEASURE_HIGH_PRECISION_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	wait_ms(9);

	uint8_t data[6];
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint8_t dataTemp[2];
	uint8_t dataRH[2];
	dataTemp[0] = data[0];
	dataTemp[1] = data[1];
	dataRH[0] = data[3];
	dataRH[1] = data[4];

	uint8_t temp_CRC = data[2];
	uint8_t rh_CRC = data[5];

	uint8_t tempDataValid = check_CRC(dataTemp, 2, temp_CRC);
	uint8_t rhDataValid = check_CRC(dataRH, 2, rh_CRC);

	if (tempDataValid && rhDataValid){
		uint16_t rawTemp = data[0] * 256 + data[1];
		uint16_t rawRH = data[3] * 256 + data[4];
		dataArray[0] = -45.0f + 175.0f * ((float)rawTemp / 65535.0f);
		dataArray[1] = -6.0f + 125.0f * ((float)rawRH/ 65535.0f);
		if (dataArray[1] > 100.0){ 
			dataArray[1] = 100.0; 
		} 
		else if (dataArray[1] < 0.0){ 
			dataArray[1] = 0.0; 
		}
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t Temp_RH_read_MediumPrecision(float *dataArray){
	uint8_t cmd = MEASURE_MEDIUM_PRECISION_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	wait_ms(5);

	uint8_t data[6];
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint8_t dataTemp[2];
	uint8_t dataRH[2];
	dataTemp[0] = data[0];
	dataTemp[1] = data[1];
	dataRH[0] = data[3];
	dataRH[1] = data[4];

	uint8_t temp_CRC = data[2];
	uint8_t rh_CRC = data[5];

	uint8_t tempDataValid = check_CRC(dataTemp, 2, temp_CRC);
	uint8_t rhDataValid = check_CRC(dataRH, 2, rh_CRC);

	if (tempDataValid && rhDataValid){
		uint16_t rawTemp = data[0] * 256 + data[1];
		uint16_t rawRH = data[3] * 256 + data[4];

		dataArray[0] = -45.0f + 175.0f * ((float)rawTemp / 65535.0f);
		dataArray[1] = -6.0f + 125.0f * ((float)rawRH/ 65535.0f);
		if (dataArray[1] > 100.0){ 
			dataArray[1] = 100.0; 
		} 
		else if (dataArray[1] < 0.0){ 
			dataArray[1] = 0.0; 
		}
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t Temp_RH_read_LowPrecision(float *dataArray){
	uint8_t cmd = MEASURE_LOW_PRECISION_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	wait_ms(2);

	uint8_t data[6];
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint8_t dataTemp[2];
	uint8_t dataRH[2];
	dataTemp[0] = data[0];
	dataTemp[1] = data[1];
	dataRH[0] = data[3];
	dataRH[1] = data[4];
	
	uint8_t temp_CRC = data[2];
	uint8_t rh_CRC = data[5];

	uint8_t tempDataValid = check_CRC(dataTemp, 2, temp_CRC);
	uint8_t rhDataValid = check_CRC(dataRH, 2, rh_CRC);
	
	if (tempDataValid && rhDataValid){
		uint16_t rawTemp = data[0] * 256 + data[1];
		uint16_t rawRH = data[3] * 256 + data[4];
		dataArray[0] = -45.0f + 175.0f * ((float)rawTemp / 65535.0f);
		dataArray[1] = -6.0f + 125.0f * ((float)rawRH/ 65535.0f);
		if (dataArray[1] > 100.0){ 
			dataArray[1] = 100.0; 
		} 
		else if (dataArray[1] < 0.0){ 
			dataArray[1] = 0.0; 
		}
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t SHT4X_useHeater(float *readings, SHT4X_HeatPower_t power, SHT4X_HeatDuration_t duration){ 
	/* 
	* This function uses the heater provided by the sensor. The workflow is explained below: 
	* 1. The heater is enabled, and the timer starts its count-down. 
	* 2. On timer expiration a temperature and humidity measurement with the highest repeatability is started, the heater remains enabled. 
	* 3. After the measurement is finished the heater is turned off. 
	* 4. Temperature and humidity values are now available for readout. 
	* Datasheet of the sensor specifies that the use of the heater MUST be less than 10% of the total lifetime of the sensor (MAX Duty Cycle = 10).
	* The function stores the values of the temperature and RH in the parameter 'data'.
	* Still missing to implement CRC
	*/

	uint8_t cmd = 0;

	switch(power){ 
	case 0: //20mW 
		if (duration == Heater_1s){ 
			cmd = HEATER_20mW_1s_cmd;
		}
		else if(duration == Heater_100ms){ 
			cmd = HEATER_20mW_100ms_cmd;
		}  
		break;

	case 1: //110 mW 
		if (duration == Heater_1s){ 
			cmd = HEATER_110mW_1s_cmd;
		}
		else if(duration == Heater_100ms){ 
			cmd = HEATER_110mW_100ms_cmd;
		}
		break; 
 
	case 2: //200mW 
		if (duration == Heater_1s){ 
			cmd = HEATER_200mW_1s_cmd;
		}
		else if(duration == Heater_100ms){ 
			cmd = HEATER_200mW_100ms_cmd;
		}
		break;  
	default: 
		break; 
	}
	
	if (cmd == 0){
		return 0;
	}
	
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000); 
	wait_ms(duration + 9); 
		
	uint8_t data[6]; 
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint8_t dataTemp[2];
	uint8_t dataRH[2];
	dataTemp[0] = data[0];
	dataTemp[1] = data[1];
	dataRH[0] = data[3];
	dataRH[1] = data[4];
		
	uint8_t temp_CRC = data[2];
	uint8_t rh_CRC = data[5];
	
	uint8_t tempDataValid = check_CRC(dataTemp, 2, temp_CRC);
	uint8_t rhDataValid = check_CRC(dataRH, 2, rh_CRC);

	if (tempDataValid && rhDataValid){
		uint16_t rawTemp = data[0] * 256 + data[1]; 
		uint16_t rawRH = data[3] * 256 + data[4]; 
		float TempCelsius = -45.0f + 175.0f * ((float)rawTemp / 65535.0f); 
		float percentageRH = -6.0f + 125.0f * ((float)rawRH / 65535.0f); 
		*(readings) = TempCelsius; 
		*(readings+1) = percentageRH;
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t SHT4X_getSerialNumber(uint32_t *var){
	uint8_t cmd = READ_SERIAL_NUMBER_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	uint8_t data[6]; 
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint8_t high_data[2];
	uint8_t low_data[2];

	high_data[0] = data[0];
	high_data[1] = data[1];
	uint8_t high_CRC = data[2];

	low_data[0] = data[3];
	low_data[1] = data[4];
	uint8_t low_CRC = data[5];

	uint8_t highDataValid = check_CRC(high_data, 2, high_CRC);
	uint8_t lowDataValid = check_CRC(low_data, 2, low_CRC);

	if (highDataValid && lowDataValid){
		*(var) = (data[0] << 24) +  (data[1] << 16) + (data[3] << 8) + (data[4]);
		return 1;
	}
	else {
		return 0;
	}
}

void SHT4X_SoftReset(void){
	uint8_t cmd = SOFT_RESET_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	wait_ms(1);
}

uint8_t check_CRC(uint8_t *data, uint8_t length, uint8_t crc_received){
    uint8_t crc = 0xFF;

    for (uint8_t i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ 0x31;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return (crc == crc_received);
}
