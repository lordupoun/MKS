/*
 * sct.h
 *
 *  Created on: Oct 10, 2024
 *      Author: 240141
 */
#include "main.h"

#ifndef INC_SCT_H_
#define INC_SCT_H_

void sct_init(void);

void sct_show(uint32_t value);

void sct_ledToggle(uint8_t led);

void sct_ledOn(uint8_t led);

void sct_ledOff(uint8_t led);

void sct_value(uint16_t value, uint8_t led, uint8_t dec);

void sct_valueWithLEDs(uint16_t value);

void sct_valueWithLEDsSpecial(uint16_t value);

void sct_turnOffSegm();

void sct_segmDice(uint8_t value);

static const uint32_t reg_values[5][10];

//static uint32_t ledReg;

//static uint32_t segmReg;

#endif /* INC_SCT_H_ */
