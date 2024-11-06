/*
 * accel.h
 *
 *  Created on: Nov 6, 2024
 *      Author: 240598
 */

#ifndef INC_ACCEL_H_
#define INC_ACCEL_H_

#include "main.h"
#include "i2c.h"
#include "lis2dw12_reg.h"


int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len);
int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);

extern stmdev_ctx_t lis2dw12;

#endif /* INC_ACCEL_H_ */
