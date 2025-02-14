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

void sct_led(uint32_t value);

void sct_value(uint16_t value, uint8_t led, uint8_t dec);

static const uint32_t reg_values[4][10];

#endif /* INC_SCT_H_ */
