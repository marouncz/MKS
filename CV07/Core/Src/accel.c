/*
 * accel.c
 *
 *  Created on: Nov 6, 2024
 *      Author: 240598
 */

#include "main.h"
#include "i2c.h"
#include "lis2dw12_reg.h"
#include "accel.h"

int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len)
{
	HAL_I2C_Mem_Write(handle, LIS2DW12_I2C_ADD_H, reg, I2C_MEMADD_SIZE_8BIT, (uint8_t *)bufp, len, 1000);
	return 0;
}
int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
	HAL_I2C_Mem_Read(handle, LIS2DW12_I2C_ADD_H, reg, I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
	return 0;
}

stmdev_ctx_t lis2dw12 =
{ .write_reg = platform_write, .read_reg = platform_read, .handle = &hi2c1 };

