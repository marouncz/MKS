/*
 * eeprom.h
 *
 *  Created on: Oct 23, 2024
 *      Author: marou
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#define EEPROM_ADDR 0b10100000

uint8_t eepromReadAddress(uint16_t memAddress);

void eepromWriteAddress(uint16_t memAddress, uint8_t data);

void eeprom16byteDump(uint16_t startAddress, uint8_t *pReadData);

#endif /* INC_EEPROM_H_ */
