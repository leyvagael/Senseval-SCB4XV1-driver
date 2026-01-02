/*
 * scb4xv1.h
 *
 *  Created on: Dec 22, 2025
 *      Author: leyva
 */

#ifndef INC_SCB4XV1_H_
#define INC_SCB4XV1_H_

#include "main.h"

/*
 * SENSEVAL SCB4XV1 board is  a 3-sensor integrated circuit. It communicates using the I2C protocol.
 * Every sensor's I2C address is included below:
 *
 * SHT4X: Measures RH and Temperature. Address = 0x44 (could be 0x45 or 0x46 as well)
 * LPS22DF: Measures Barometric pressure. Address =
 * SGP40: Measures Volatile Organic Compounds (VOC). Address =
 *
*/

#include "main.h"

/* Device I2C Addresses */
#define SHT4X_Address					0x44<<1

/* SHT4X command defines */
#define MEASURE_HIGH_PRECISION_cmd		0xFD
#define MEASURE_MEDIUM_PRECISION_cmd		0xF6
#define MEASURE_LOW_PRECISION_cmd		0xE0
#define READ_SERIAL_NUMBER_cmd			0x89
#define SOFT_RESET_cmd				0x94
#define HEATER_200mW_1s_cmd			0x39
#define HEATER_200mW_100ms_cmd			0x32
#define HEATER_110mW_1s_cmd			0x2F
#define HEATER_110mW_100ms_cmd			0x24
#define HEATER_20mW_1s_cmd			0x1E
#define HEATER_20mW_100ms_cmd			0x15

/* SHT4X typedef enums for using the heater */
typedef enum {
	Heater_20mW,
	Heater_110mW,
	Heater_200mW
} SHT4X_HeatPower_t;

typedef enum {
	Heater_1s = 1000,
	Heater_100ms = 100
} SHT4X_HeatDuration_t;

/* Sensirion INIT function */
void scb4xv1_init(I2C_HandleTypeDef *hi2c, TIM_HandleTypeDef *htim);

/* Main SHT4X functions */
uint8_t getSerialNumber(void);
void SHT4X_SoftReset(void);

uint8_t readRaw_HighPrecision(uint16_t *dataArray);
uint8_t readRaw_MediumPrecision(uint16_t *dataArray);
uint8_t readRaw_LowPrecision(uint16_t *dataArray);

uint8_t read_HighPrecision(float *dataArray);
uint8_t read_MediumPrecision(float *dataArray);
uint8_t read_LowPrecision(float *dataArray);

uint8_t useHeater(float *readings, SHT4X_HeatPower_t power, SHT4X_HeatDuration_t duration);

#endif /* INC_SCB4XV1_H_ */