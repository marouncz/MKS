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
#include "fsl_ctimer.h"
#include "peripherals.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/


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
    BOARD_InitBootPeripherals();
    CTIMER_StartTimer(CTIMER1_PERIPHERAL);
    LED_BLUE_INIT(1);

#if !defined(DONT_ENABLE_FLASH_PREFETCH)
    /* enable flash prefetch for better performance */
    SYSCON->FMCCR |= SYSCON_FMCCR_PREFEN_MASK;
#endif

    PRINTF("hello world.\r\n");

    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}

void TimerIRQ(uint32_t flags)
{
	PRINTF("hello from ISR.\r\n");
	//GPIO_PortToggle(LEDG, port, mask)
	LED_BLUE_TOGGLE();

}
