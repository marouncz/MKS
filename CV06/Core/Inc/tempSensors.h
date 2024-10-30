/*
 * tempSensors.h
 *
 *  Created on: Oct 30, 2024
 *      Author: 240598
 */

#ifndef INC_TEMPSENSORS_H_
#define INC_TEMPSENSORS_H_

#include "main.h"

void processDallas(void);

void getDallasTemp(int16_t *temperature);

void processNTC(void);

void getNTCTemp(int16_t *temperature);


#endif /* INC_TEMPSENSORS_H_ */
