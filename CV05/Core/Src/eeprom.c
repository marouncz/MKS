/*
 * eeprom.c
 *
 *  Created on: Oct 23, 2024
 *      Author: marou
 */

#include "main.h"
#include "i2c.h"
#include "eeprom.h"

uint8_t eepromReadAddress(uint16_t memAddress)
{

	uint8_t dataRead;
	HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, memAddress, I2C_MEMADD_SIZE_16BIT, &dataRead, 1, 1000);
	return dataRead;
}

void eepromWriteAddress(uint16_t memAddress, uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, memAddress, I2C_MEMADD_SIZE_16BIT, &data, 1, 1000);
	while (HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR, 300, 1000) == HAL_TIMEOUT)
	{
	}

}

void eeprom16byteDump(uint16_t startAddress, uint8_t *pReadData)
{
	HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, startAddress, I2C_MEMADD_SIZE_16BIT, pReadData, 16, 1000);

}
