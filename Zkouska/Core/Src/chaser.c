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
#define MAX_IN 4095
#define MIN_OUT 20
#define MAX_OUT 500

#define RIDER_LEDS 4

uint32_t avg_pot;
uint32_t raw_pot;
uint32_t setDelay = 20;

uint32_t chaserState = (0b1<<RIDER_LEDS)-1;



uint16_t calculate_delay(void) {
    // Apply the formula to map variable to the delay range
	uint16_t delay = MIN_OUT + ((raw_pot - MIN_IN) * (MAX_OUT - MIN_OUT)) / (MAX_IN - MIN_IN);
    return delay;
}

void handleTimer(void)
{
	raw_pot = avg_pot >> ADC_Q;
	avg_pot -= raw_pot;
	avg_pot += HAL_ADC_GetValue(&hadc);
	setDelay = calculate_delay();
	//sct_value_binary_led(setDelay, 0b11000011);
}

void handleChaser(void)
{
	for(uint8_t i = 0; i < (6+RIDER_LEDS); i++)
	{
		chaserState <<= 1;
		sct_value_binary_led(setDelay, chaserState>>(RIDER_LEDS-1));
		HAL_Delay(setDelay);
	}
	for(uint8_t i = 0; i < (6+RIDER_LEDS); i++)
	{
		chaserState >>= 1;
		sct_value_binary_led(setDelay, chaserState>>(RIDER_LEDS-1));
		HAL_Delay(setDelay);
	}



}


