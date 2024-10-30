/*
 * uart_processing.c
 *
 *  Created on: Oct 22, 2024
 *      Author: marou
 */
#include "uart_processing.h"
#include "main.h"
#include <stdio.h>
#include "usart.h"
#include <string.h>
#include "eeprom.h"
#include <stdlib.h>

void uart_byte_available(uint8_t c)
{
	static uint16_t cnt;
	static char data[CMD_BUFFER_LEN];
	if (cnt < CMD_BUFFER_LEN && c >= 32 && c <= 126)
		data[cnt++] = c;
	if ((c == '\n' || c == '\r') && cnt > 0)
	{
		data[cnt] = '\0';
		uart_process_command(&data[0]);
		printf("received: '%s'\r\n", data);
		cnt = 0;
	}
}

void uart_process_command(char *data)
{
	char *token;
	token = strtok(data, " ");
	if (strcasecmp(token, "HELLO") == 0)
	{
		printf("Communication OK\r\n");
	}
	else if (strcasecmp(token, "LED1") == 0)
	{
		token = strtok(NULL, " ");
		if (strcasecmp(token, "ON") == 0)
		{
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		}
		else if (strcasecmp(token, "OFF") == 0)
		{
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		}
		printf("OK\r\n");
	}
	else if (strcasecmp(token, "LED2") == 0)
	{
		token = strtok(NULL, " ");
		if (strcasecmp(token, "ON") == 0)
		{
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		}
		else if (strcasecmp(token, "OFF") == 0)
		{
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		}
		printf("OK\r\n");
	}
	if (strcasecmp(token, "STATUS") == 0)
	{
		uint8_t statusLED1 = HAL_GPIO_ReadPin(LED1_GPIO_Port, LED1_Pin);
		uint8_t statusLED2 = HAL_GPIO_ReadPin(LED2_GPIO_Port, LED2_Pin);
		printf("LED1 is %s, LED2 is %s\r\n", statusLED1 ? "ON" : "OFF", statusLED2 ? "ON" : "OFF");
	}
	if (strcasecmp(token, "READ") == 0)
	{
		token = strtok(NULL, " ");
		uint8_t dataRead;
		dataRead = eepromReadAddress(atoi(token));
		printf("Address: 0x%04X, Data: 0x%02X\r\n", atoi(token), dataRead);
	}
	if (strcasecmp(token, "WRITE") == 0)
	{
		token = strtok(NULL, " ");
		uint16_t addressToWrite = atoi(token);
		token = strtok(NULL, " ");
		uint8_t dataToWrite = atoi(token);
		eepromWriteAddress(addressToWrite, dataToWrite);
		printf("OK\r\n");
	}
	if (strcasecmp(token, "DUMP") == 0)
	{
		uint8_t dumpData[16];
		eeprom16byteDump(0x00, &dumpData[0]);
	    printf(
	        "%02X %02X %02X %02X %02X %02X %02X %02X\r\n"
	        "%02X %02X %02X %02X %02X %02X %02X %02X\r\n",
	        dumpData[0], dumpData[1], dumpData[2], dumpData[3],
	        dumpData[4], dumpData[5], dumpData[6], dumpData[7],
	        dumpData[8], dumpData[9], dumpData[10], dumpData[11],
	        dumpData[12], dumpData[13], dumpData[14], dumpData[15]
	    );
	}


}

int _write(int file, char const *buf, int n)
{
	/* stdout redirection to UART2 */
	HAL_UART_Transmit(&huart2, (uint8_t*) (buf), n, HAL_MAX_DELAY);
	return n;
}
