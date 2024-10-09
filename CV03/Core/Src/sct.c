/*
 * sct.c
 *
 *  Created on: Oct 9, 2024
 *      Author: 240598
 */

#include "main.h"
#include "sct.h"

void sct_init(void)
{
	//HAL_GPIO_WritePin(SCT_NOE_GPIO_Port, SCT_NOE_Pin, GPIO_PIN_RESET);
	sct_led(0);
}

void sct_led(uint32_t value)
{
	for(uint8_t i = 0; i <32; i++)
	{
		HAL_GPIO_WritePin(SCT_SDI_GPIO_Port, SCT_SDI_Pin, (value & 1)); //send data bit by bit
		value >>= 1;
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, 1); //generate a clock pulse
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, 0);

	}

	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, 1); //latch output data
	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, 0);

}


