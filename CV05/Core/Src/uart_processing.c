/*
 * uart_processing.c
 *
 *  Created on: Oct 22, 2024
 *      Author: marou
 */
#include "uart_processing.h"
#include "main.h"
#include "stdio.h"
#include "usart.h"

void uart_byte_available(uint8_t c) {
	static uint16_t cnt;
	static char data[CMD_BUFFER_LEN];
	if (cnt < CMD_BUFFER_LEN && c >= 32 && c <= 126)
		data[cnt++] = c;
	if ((c == '\n' || c == '\r') && cnt > 0) {
		data[cnt] = '\0';
		//uart_process_command(data);
		printf("received: '%s'\r\n", data);
		cnt = 0;
	}
}


int _write(int file, char const *buf, int n)
{
 /* stdout redirection to UART2 */
 HAL_UART_Transmit(&huart2, (uint8_t*)(buf), n, HAL_MAX_DELAY);
 return n;
}
