/*
 * uart_processing.h
 *
 *  Created on: Oct 22, 2024
 *      Author: marou
 */

#ifndef INC_UART_PROCESSING_H_
#define INC_UART_PROCESSING_H_

#include "main.h"

#define CMD_BUFFER_LEN 256

void uart_byte_available(uint8_t c);


#endif /* INC_UART_PROCESSING_H_ */
