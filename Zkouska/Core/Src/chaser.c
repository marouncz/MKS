/*
 * chaser.c
 *
 *  Created on: Dec 18, 2024
 *      Author: 240598
 */

#include "main.h"
#include "sct.h"
#include "adc.h"

#define ADC_Q 4

#define MIN_IN 0
#define MAX_IN 4090
#define MIN_OUT 20
#define MAX_OUT 500

#define RIDER_LEDS 3

uint32_t avg_pot;
uint32_t raw_pot;
uint32_t setDelay;

//this variable represents current led state that should be on in binary
uint32_t chaserState = (0b1 << RIDER_LEDS) - 1;

uint16_t calculate_delay(void)
{
	// Apply the formula to map variable to the delay range
	uint16_t delay = MIN_OUT + ((raw_pot - MIN_IN) * (MAX_OUT - MIN_OUT)) / (MAX_IN - MIN_IN);
	if(delay > 500)
	{
		delay = 500;
	}
	return delay;
}

void initChaserADC(void)
{
	//fill in the inital values of EMA filter so it doesnt take a long time to average out
	raw_pot = HAL_ADC_GetValue(&hadc);
	avg_pot = raw_pot << ADC_Q;
}

void handleTimer(void)
{
	//timer handler for averaging ADC values and handeling displays
	raw_pot = avg_pot >> ADC_Q;
	avg_pot -= raw_pot;
	avg_pot += HAL_ADC_GetValue(&hadc);

	setDelay = calculate_delay();

	sct_value_binary_led(setDelay, chaserState >> (RIDER_LEDS - 1));
}

void handleChaser(void)
{
	//knight rider one direction
	for (uint8_t i = 0; i < (6 + RIDER_LEDS); i++)
	{
		chaserState <<= 1;
		HAL_Delay(setDelay);
	}
	//knight rider other direction
	for (uint8_t i = 0; i < (6 + RIDER_LEDS); i++)
	{
		chaserState >>= 1;
		HAL_Delay(setDelay);
	}

}

