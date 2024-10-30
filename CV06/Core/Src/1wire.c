/*************************************************************************
 *
 * Simple 1-wire driver for DS18B20 sensor
 * (c) 2007-2020  Ales Povalac
 *
 *************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "1wire.h"


/******************************************************************
 * LOW LEVEL functions for 1-wire bus
 ******************************************************************/

void OWInit(void)
{
    OWSetLink(1);
    OWInitHw();

    OWReset();
}

uint8_t OWReset(void)
{
    uint8_t result;

    DELAY_G;
    __disable_irq();
    OWSetLink(0);
    DELAY_H;
    OWSetLink(1);
    DELAY_I;
    result = OWGetLink();
    __enable_irq();
    DELAY_J;
    return result;
}

void OWWriteBit(uint8_t bit)
{
    __disable_irq();
    if (bit) {
        OWSetLink(0);
        DELAY_A;
        OWSetLink(1);
        DELAY_B;
    } else {
        OWSetLink(0);
        DELAY_C;
        OWSetLink(1);
        DELAY_D;
    }
    __enable_irq();
}

uint8_t OWReadBit(void)
{
    uint8_t result;

    __disable_irq();
    OWSetLink(0);
    DELAY_A;
    OWSetLink(1);
    DELAY_E;
    result = OWGetLink();
    __enable_irq();
    DELAY_F;
    return result;
}

void OWWriteByte(uint8_t data)
{
    uint8_t i;

    for (i = 0; i < 8; i++) {
        OWWriteBit(data & 0x01);
        data >>= 1;
    }
}

uint8_t OWReadByte(void)
{
    int i, result = 0;

    for (i = 0; i < 8; i++) {
        result >>= 1;
        if (OWReadBit())
            result |= 0x80;
    }
    return result;
}

void OWSendCmd(uint8_t *ROM, uint8_t cmd)
{
    uint8_t i;

    OWReset();
    if (ROM == NULL) {
        OWWriteByte(OW_SKIP_ROM_CMD);
    } else {
        OWWriteByte(OW_MATCH_ROM_CMD);
        for (i = 0; i < 8; i++)
            OWWriteByte(ROM[i]);
    }
    OWWriteByte(cmd);
}

void OWCRC(uint8_t x, uint8_t *crc)
{
    for (uint8_t i = 8; i; i--) {
        uint8_t mix = (*crc ^ x) & 0x01;
        *crc >>= 1;
        if (mix) {
            *crc ^= 0x8C;
        }
        x >>= 1;
    }
}

/******************************************************************
 * HIGH LEVEL functions for 1-wire bus
 ******************************************************************/

uint8_t OWReadRom(uint8_t *ROM)
{
    uint8_t i, crc = 0;

    OWReset();
    OWWriteByte(OW_READ_ROM_CMD);

    for (i = 0; i < 7; i++) {
        ROM[i] = OWReadByte();
        OWCRC(ROM[i], &crc);
    }
    ROM[7] = OWReadByte();

    if (crc != ROM[7]) return 0;
    if (ROM[0] != DS18B20_SIG) return 0;

    return 1;
}

void OWConvertAll(void)
{
    OWSendCmd(NULL, OW_CONVERT_T_CMD);
}

uint8_t OWReadTemperature(int16_t *result)
{
    uint8_t i, crc = 0, buf[8];

    OWSendCmd(NULL, OW_RD_SCR_CMD);
    for (i = 0; i < 8; i++) {
        buf[i] = OWReadByte();
        OWCRC(buf[i], &crc);
    }
    if (crc != OWReadByte()) {
        *result = OW_ERR_BADCRC;
        return 0;
    }

    *result = (int8_t)((buf[1] << 4) | (buf[0] >> 4)) * 100 + (buf[0] & 0x0F) * 100 / 16;

    return 1;
}
