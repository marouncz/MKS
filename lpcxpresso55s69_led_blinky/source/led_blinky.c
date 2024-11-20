/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "board.h"

#include "fsl_power.h"
#include <stdio.h>
#include "fsl_debug_console.h"

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_LED_PORT BOARD_LED_BLUE_GPIO_PORT
#define BOARD_LED_PIN  BOARD_LED_BLUE_GPIO_PIN

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_systickCounter;

volatile uint32_t DWT1, DWT2;


/*******************************************************************************
 * Code
 ******************************************************************************/
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

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init output LED GPIO. */
    GPIO_PortInit(GPIO, BOARD_LED_PORT);
    /* Board pin init */
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    BOARD_InitBootPins();
    SystemCoreClockUpdate();
    BOARD_InitDebugConsole();
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

    while (1)
    {
        /* Delay 1000 ms */
        SysTick_DelayTicks(1000U);

        DWT1 = DWT->CYCCNT;
        GPIO_PortToggle(GPIO, BOARD_LED_PORT, 1u << BOARD_LED_PIN);
        DWT2 = DWT->CYCCNT;
        PRINTF("\r\nCycles in function: %d", DWT2-DWT1);
    }
}
