/*
 * sct.c
 *
 *  Created on: Oct 9, 2024
 *      Author: 240598
 */

#include "main.h"
#include "sct.h"

static const uint32_t reg_values[5][10] = {
		{
				//PCDE--------GFAB @ DIS1
				0b0111000000000111 << 16,
				0b0100000000000001 << 16,
				0b0011000000001011 << 16,
				0b0110000000001011 << 16,
				0b0100000000001101 << 16,
				0b0110000000001110 << 16,
				0b0111000000001110 << 16,
				0b0100000000000011 << 16,
				0b0111000000001111 << 16,
				0b0110000000001111 << 16,
		},
		{
				//----PCDEGFAB---- @ DIS2
				0b0000011101110000 << 0,
				0b0000010000010000 << 0,
				0b0000001110110000 << 0,
				0b0000011010110000 << 0,
				0b0000010011010000 << 0,
				0b0000011011100000 << 0,
				0b0000011111100000 << 0,
				0b0000010000110000 << 0,
				0b0000011111110000 << 0,
				0b0000011011110000 << 0,
		},
		{
				//PCDE--------GFAB @ DIS3
				0b0111000000000111 << 0,
				0b0100000000000001 << 0,
				0b0011000000001011 << 0,
				0b0110000000001011 << 0,
				0b0100000000001101 << 0,
				0b0110000000001110 << 0,
				0b0111000000001110 << 0,
				0b0100000000000011 << 0,
				0b0111000000001111 << 0,
				0b0110000000001111 << 0,
		},
		{
				//----43215678---- @ LED
				0b0000000000000000 << 16,
				0b0000000100000000 << 16,
				0b0000001100000000 << 16,
				0b0000011100000000 << 16,
				0b0000111100000000 << 16,
				0b0000111110000000 << 16,
				0b0000111111000000 << 16,
				0b0000111111100000 << 16,
				0b0000111111110000 << 16,
		},

};

static const uint32_t standalone_led_values[10] = {
		//another array for separate led control
		//----43215678---- @ LED
		0b0000000000000000 << 16,
		0b0000000100000000 << 16,
		0b0000001000000000 << 16,
		0b0000010000000000 << 16,
		0b0000100000000000 << 16,
		0b0000000010000000 << 16,
		0b0000000001000000 << 16,
		0b0000000000100000 << 16,
		0b0000000000010000 << 16,

};

void sct_init(void)
{
	HAL_GPIO_WritePin(SCT_NOE_GPIO_Port, SCT_NOE_Pin, GPIO_PIN_RESET); // Output enable
	sct_led(0);
}

void sct_led(uint32_t value)
{
	for(uint8_t i = 0; i <32; i++)
	{
		HAL_GPIO_WritePin(SCT_SDI_GPIO_Port, SCT_SDI_Pin, (value & 1)); //send data bit by bit
		value >>= 1;
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, GPIO_PIN_SET); //generate a clock pulse
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, GPIO_PIN_RESET);

	}

	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, GPIO_PIN_SET); //latch output data
	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, GPIO_PIN_RESET);

}

void sct_value(uint16_t value, uint8_t led)
{
	uint32_t reg = 0;
	reg |= reg_values[0][value / 100 % 10];
	reg |= reg_values[1][value / 10 % 10];
	reg |= reg_values[2][value / 1 % 10];
	reg |= reg_values[3][led];
	sct_led(reg);


}

void sct_value_binary_led(uint16_t value, uint8_t led)
{
	//led variable input argument is used for standalone led control. it is defined in binary, which led should be on
	uint32_t reg = 0;
	reg |= reg_values[0][value / 100 % 10];
	reg |= reg_values[1][value / 10 % 10];
	reg |= reg_values[2][value / 1 % 10];

	//look for which leds should be on and or them to the reg value
	for (uint8_t i = 0; i < 8; i++)
	{
		if (led & (0b1 << i))
		{
			reg |= standalone_led_values[i + 1];
		}
	}

	sct_led(reg);

}

