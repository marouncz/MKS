/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "fsl_power.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

volatile uint32_t g_systickCounter;
void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}
void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t DWT1, DWT2;
char password_stored[20] = "12345678";
char input[20];

int strcmp_ct(const char *a, const char *b)
{
	  int r = 0, c;
	  for (;;)
	  {
	    c = *a - *b;
	    if (!r) r = c;
	    if (!*a && !*b) break;
	    if (*a) ++a;
	    if (*b) ++b;
	  }
	  return r;
}

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    char ch;

    /* Init board hardware. */
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    SystemCoreClockUpdate();

	LED_BLUE_INIT(1);
#if !defined(DONT_ENABLE_FLASH_PREFETCH)
    /* enable flash prefetch for better performance */
    SYSCON->FMCCR |= SYSCON_FMCCR_PREFEN_MASK;
#endif

	/* Set systick reload value to generate 1ms interrupt */
	if (SysTick_Config(SystemCoreClock / 1000U))
	{
		while (1)
		{
		}
	}

    PRINTF("hello world.\r\n");

    while (1)
    {
    	uint8_t status;
    	PRINTF("\r\nEnter password: ");
    	SCANF("%s", input);
        DWT1 = DWT->CYCCNT;
        //status = strcmp(input, password_stored);
        status = strcmp_ct(input, password_stored);

        DWT2 = DWT->CYCCNT;
        PRINTF("\r\nCycles in function: %d", DWT2-DWT1);
        if(status == 0)
        {
        	PRINTF("\r\n Password correct");
        }
        else
        {
        	PRINTF("\r\n Password incorrect");
        }

        //PRINTF("\r\n Password correct");

    }
}
