/*************************************************************************
 *
 * Simple 1-wire driver for DS18B20 sensor
 * (c) 2007-2020  Ales Povalac
 *
 *************************************************************************/

#ifndef _1WIRE_H
#define _1WIRE_H

#include "main.h"

#define OWInitHw()          { }
#define OWSetLink(x)        { if (x) DQ_GPIO_Port->BSRR = DQ_Pin; else DQ_GPIO_Port->BRR = DQ_Pin; }
#define OWGetLink()         ( (DQ_GPIO_Port->IDR & DQ_Pin) ? 1 : 0 )

/* Dumb delay for F030. Tuned for default clock configuration, i.e. 48MHz with 1 wait state. */
__attribute__((always_inline))
inline static void _delay_us(volatile uint32_t micros)
{
    micros = (micros * 19) / 4; /* Go to clock cycles */
    while (micros--); /* Wait till done */
}

/* LOW LEVEL */
extern void OWInit(void);
extern uint8_t OWReset(void);
extern void OWWriteBit(uint8_t bit);
extern uint8_t OWReadBit(void);
extern void OWWriteByte(uint8_t data);
extern uint8_t OWReadByte(void);
extern void OWSendCmd(uint8_t *ROM, uint8_t cmd);
extern void OWCRC(uint8_t x, uint8_t *crc);

/* HIGH LEVEL */
extern uint8_t OWReadRom(uint8_t *ROM);
extern void OWConvertAll(void);
extern uint8_t OWReadTemperature(int16_t *temperature);

/* INTERNAL CONSTANTS everything below this line */

#define OW_ERR_BADCRC      0x8000
#define OW_ERR_BADFAMILY   0x8001

/* Return codes for OWFirst()/OWNext() */
#define OW_BADWIRE      -3
#define OW_BADCRC       -2
#define OW_NOPRESENCE   -1
#define OW_NOMODULES    0
#define OW_FOUND        1

/* General 1 wire commands */
#define OW_SEARCH_ROM_CMD   0xF0
#define OW_READ_ROM_CMD     0x33
#define OW_MATCH_ROM_CMD    0x55
#define OW_SKIP_ROM_CMD     0xCC

/* DS1820 commands */
#define OW_CONVERT_T_CMD    0x44
#define OW_RD_SCR_CMD       0xBE
#define OW_WR_SCR_CMD       0x4E

#define DS18B20_SIG         0x28

/* 1-wire delays */
#define DELAY_A _delay_us(4)
#define DELAY_B _delay_us(66)
#define DELAY_C _delay_us(65)
#define DELAY_D _delay_us(5)
#define DELAY_E _delay_us(9)
#define DELAY_F _delay_us(55)
#define DELAY_G
#define DELAY_H _delay_us(510)
#define DELAY_I _delay_us(70)
#define DELAY_J _delay_us(3340)

/* Other */
#define CONVERT_T_DELAY 750

#endif
