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

void wait_ms(uint8_t ms){
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
uint8_t readRaw_HighPrecision(uint16_t *dataArray){
	uint8_t cmd = MEASURE_HIGH_PRECISION_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	wait_ms(9);

	uint8_t data[6];
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint16_t rawTemp = data[0] * 256 + data[1];
	uint8_t temp_CRC = data[2];
	uint16_t rawRH = data[3] * 256 + data[4];
	uint8_t rh_CRC = data[5];

	uint8_t tempDataValid = check_CRC(temp_CRC);
	uint8_t rhDataValid = check_CRC(rh_CRC);
	if (!(tempDataValid && rhDataValid)){
		dataArray[0] = rawTemp;
		dataArray[1] = rawRH;
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t readRaw_MediumPrecision(uint16_t *dataArray){
	uint8_t cmd = MEASURE_MEDIUM_PRECISION_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	wait_ms(5);

	uint8_t data[6];
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint16_t rawTemp = data[0] * 256 + data[1];
	uint8_t temp_CRC = data[2];
	uint16_t rawRH = data[3] * 256 + data[4];
	uint8_t rh_CRC = data[5];

	uint8_t tempDataValid = check_CRC(temp_CRC);
	uint8_t rhDataValid = check_CRC(rh_CRC);
	if (tempDataValid && rhDataValid){
		dataArray[0] = rawTemp;
		dataArray[1] = rawRH;
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t readRaw_LowPrecision(uint16_t *dataArray){
	uint8_t cmd = MEASURE_LOW_PRECISION_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	wait_ms(2);

	uint8_t data[6];
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint16_t rawTemp = data[0] * 256 + data[1];
	uint8_t temp_CRC = data[2];
	uint16_t rawRH = data[3] * 256 + data[4];
	uint8_t rh_CRC = data[5];

	uint8_t tempDataValid = check_CRC(temp_CRC);
	uint8_t rhDataValid = check_CRC(rh_CRC);
	if (tempDataValid && rhDataValid){
		dataArray[0] = rawTemp;
		dataArray[1] = rawRH;
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t read_HighPrecision(float *dataArray){
	uint8_t cmd = MEASURE_HIGH_PRECISION_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	wait_ms(9);

	uint8_t data[6];
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint16_t rawTemp = data[0] * 256 + data[1];
	uint8_t temp_CRC = data[2];
	uint16_t rawRH = data[3] * 256 + data[4];
	uint8_t rh_CRC = data[5];

	uint8_t tempDataValid = check_CRC(temp_CRC);
	uint8_t rhDataValid = check_CRC(rh_CRC);
	if (!(tempDataValid && rhDataValid)){
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

uint8_t read_MediumPrecision(float *dataArray){
	uint8_t cmd = MEASURE_MEDIUM_PRECISION_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	wait_ms(5);

	uint8_t data[6];
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint16_t rawTemp = data[0] * 256 + data[1];
	uint8_t temp_CRC = data[2];
	uint16_t rawRH = data[3] * 256 + data[4];
	uint8_t rh_CRC = data[5];

	uint8_t tempDataValid = check_CRC(temp_CRC);
	uint8_t rhDataValid = check_CRC(rh_CRC);
	if (!(tempDataValid && rhDataValid)){
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

uint8_t read_LowPrecision(float *dataArray){
	uint8_t cmd = MEASURE_LOW_PRECISION_cmd;
	HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000);
	wait_ms(2);

	uint8_t data[6];
	HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000);

	uint16_t rawTemp = data[0] * 256 + data[1];
	uint8_t temp_CRC = data[2];
	uint16_t rawRH = data[3] * 256 + data[4];
	uint8_t rh_CRC = data[5];

	uint8_t tempDataValid = check_CRC(temp_CRC);
	uint8_t rhDataValid = check_CRC(rh_CRC);
	if (!(tempDataValid && rhDataValid)){
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

void useHeater(float* readings, uint8_t power, uint8_t duration){ 
	/* 
	* This function uses the heater provided by the sensor. The workflow is explained below: * 1. The heater is enabled, and the timer starts its count-down. 
	* 2. On timer expiration a temperature and humidity measurement with the highest repeatability is started, the heater remains enabled. 
	* 3. After the measurement is finished the heater is turned off. 
	* 4. Temperature and humidity values are now available for readout. 
	* Datasheet of the sensor specifies that the use of the heater MUST be less than 10% of the total lifetime of the sensor (MAX Duty Cycle = 10).
	* The function stores the values of the temperature and RH in the parameter 'data'.
	* Still missing to implement CRC
	*/ 
	switch(power){ 
	case 1: //20mW 
		if (duration == 1000){ 
			uint8_t cmd = HEATER_20mW_1s_cmd; 
			HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000); 
			wait_ms(duration + 9); 
			uint8_t data[6]; 
			HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000); 
			uint16_t rawTemp = data[0] * 256 + data[1]; 
			uint16_t rawRH = data[3] * 256 + data[4]; 
			float TempCelsius = -45.0f + 175.0f * ((float)rawTemp / 65535.0f); 
			float percentageRH = -6.0f + 125.0f * ((float)rawRH / 65535.0f); 
			*(readings) = TempCelsius; 
			*(readings+1) = percentageRH;
		} 
		else if(duration == 100){ 
			uint8_t cmd = HEATER_20mW_100ms_cmd; 
			HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000); 
			wait_ms(duration + 9); 
			uint8_t data[6]; 
			HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000); 
			uint16_t rawTemp = data[0] * 256 + data[1]; 
			uint16_t rawRH = data[3] * 256 + data[4]; 
			float TempCelsius = -45.0f + 175.0f * ((float)rawTemp / 65535.0f); 
			float percentageRH = -6.0f + 125.0f * ((float)rawRH / 65535.0f); 
			*(readings) = TempCelsius; 
			*(readings+1) = percentageRH; 
		} 
		break; 
	case 2: //110 mW 
		if (duration == 1000){ 
			uint8_t cmd = HEATER_110mW_1s_cmd; 
			HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000); 
			wait_ms(duration + 9); 
			uint8_t data[6]; HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000); 
			uint16_t rawTemp = data[0] * 256 + data[1]; 
			uint16_t rawRH = data[3] * 256 + data[4]; 
			float TempCelsius = -45.0f + 175.0f * ((float)rawTemp / 65535.0f); 
			float percentageRH = -6.0f + 125.0f * ((float)rawRH / 65535.0f); 
			*(readings) = TempCelsius; 
			*(readings+1) = percentageRH; 
		} 
		else if(duration == 100){ 
			uint8_t cmd = HEATER_110mW_100ms_cmd; 
			HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000); 
			wait_ms(duration + 9); 
			uint8_t data[6]; 
			HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000); 
			uint16_t rawTemp = data[0] * 256 + data[1]; 
			uint16_t rawRH = data[3] * 256 + data[4]; 
			float TempCelsius = -45.0f + 175.0f * ((float)rawTemp / 65535.0f); 
			float percentageRH = -6.0f + 125.0f * ((float)rawRH / 65535.0f); 
			*(readings) = TempCelsius; 
			*(readings+1) = percentageRH; 
		} 
		break; 
	case 3: //200mW 
		if (duration == 1000){ 
			uint8_t cmd = HEATER_110mW_1s_cmd; 
			HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000); 
			wait_ms(duration + 9); 

			uint8_t data[6]; HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000); 
			uint16_t rawTemp = data[0] * 256 + data[1];
			uint8_t temp_CRC = data[2]; 
			uint16_t rawRH = data[3] * 256 + data[4]; 
			uint8_t rh_CRC = data[5];
			
			float TempCelsius = -45.0f + 175.0f * ((float)rawTemp / 65535.0f); 
			float percentageRH = -6.0f + 125.0f * ((float)rawRH / 65535.0f); 
			*(readings) = TempCelsius; 
			*(readings+1) = percentageRH; 
		}
		else if(duration == 100){ 
			uint8_t cmd = HEATER_110mW_100ms_cmd; 
			HAL_I2C_Master_Transmit(hi2c_scb4xv1, SHT4X_Address, &cmd, 1, 1000); 
			wait_ms(duration + 9); 
			uint8_t data[6]; 
			HAL_I2C_Master_Receive (hi2c_scb4xv1, SHT4X_Address, data, 6, 1000); 
			uint16_t rawTemp = data[0] * 256 + data[1]; 
			uint16_t rawRH = data[3] * 256 + data[4]; 
			float TempCelsius = -45.0f + 175.0f * ((float)rawTemp / 65535.0f); 
			float percentageRH = -6.0f + 125.0f * ((float)rawRH / 65535.0f); 
			*(readings) = TempCelsius; 
			*(readings+1) = percentageRH;
		}
		break; 
	default: 
		break; 
	
	return; 
}