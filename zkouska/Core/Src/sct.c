/*
 * sct.c
 *
 *  Created on: Oct 10, 2024
 *      Author: 240141
 */
#include "sct.h"

static uint32_t ledReg=0;
static uint32_t segmReg=0;

//inicializace
void sct_init(void)
{
	sct_show(0);
}

//vysle hodnotu na IO
void sct_show(uint32_t value)
{
	for(uint8_t i=0; i<32;i++)
	{
		HAL_GPIO_WritePin(SCT_SDI_GPIO_Port, SCT_SDI_Pin , (value>>i)%2);
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, 1);
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, 0);
	}
	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, 1);
	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, 0);

	HAL_GPIO_WritePin(SCT_NOE_GPIO_Port, SCT_NOE_Pin, 0);//povolen zapis
}

//Zobrazi konkretni hodnotu s pozicemi LED a desetinnou carkou, prepise vsechny puvodni hodnoty
/**
 * dec = 1 - 1 desetinne misto; 2 - 2 desetinna mista; 100 - tecka na konci
 */
void sct_value(uint16_t value, uint8_t led, uint8_t dec)
{
	uint32_t reg=0;
	reg |= reg_values[0][value / 100 % 10]; //stovky
	reg |= reg_values[1][value / 10 % 10]; //desitky
	reg |= reg_values[2][value % 10]; //jednotky

	reg |=reg_values[4][led];

	switch(dec)
	{
	case 1:
		reg |=0b0000100000000000 << 0;
	break;

	case 2:
		reg |=0b1000000000000000 << 16;
	break;

	case 100:
		reg |=0b1000000000000000 << 0;
	break;
	}

	sct_show(reg);
}
//Zobrazi hodnotu bez smazani stavu LED
void sct_valueWithLEDs(uint16_t value)
{
	segmReg=0;
	segmReg |= reg_values[0][value / 100 % 10]; //stovky
	segmReg |= reg_values[1][value / 10 % 10]; //desitky
	segmReg |= reg_values[2][value % 10]; //jednotky
	segmReg |=ledReg; //aby se odeslal i s LED
	sct_show(segmReg);
}
//Zobrazi hodnotu bez smazani stavu LED pouze na platnych displejich
void sct_valueWithLEDsSpecial(uint16_t value)
{
	segmReg=0;
	if(value / 100 % 10!=0)
	segmReg |= reg_values[0][value / 100 % 10]; //stovky
	if(value / 10 % 10!=0||value / 100 % 10!=0)
	segmReg |= reg_values[1][value / 10 % 10]; //desitky
	segmReg |= reg_values[2][value % 10]; //jednotky
	segmReg |=ledReg; //aby se odeslal i s LED
	sct_show(segmReg);
}
//Rozsviti konkretni LED diodu (ostatni nezhasnou)
void sct_ledOn(uint8_t led)
{
	segmReg=0;
	ledReg |=reg_values[3][led];
	segmReg|=ledReg; //pozor na spravnou posloupnost! ledReg|= byt nemuze protoze registr LED do kteryho se vkladaji segmenty se nevycisti
	sct_show(segmReg);
}
//Zhasne konkretni LED diodu
void sct_ledOff(uint8_t led)
{
	segmReg=0;
	ledReg &=~reg_values[3][led]; //AND s negací vynuluje
	segmReg|=ledReg;
	sct_show(segmReg);
}
//Toggle konkretni LED diody
void sct_ledToggle(uint8_t led)
{
	segmReg=0;
	ledReg ^=reg_values[3][led]; //XOR udělá toggle
	segmReg|=ledReg;
	sct_show(segmReg);
}
//Vypne vsechny segmenty displeje
void sct_turnOffSegm()
{
	segmReg=0;
	segmReg |=ledReg; //aby se odeslal i s LED
	sct_show(segmReg);
}
//Zobrazi hodnotu 1-6 jako na kostce
void sct_segmDice(uint8_t value)
{
	uint32_t diceReg=0;
	switch(value)
	{
	case 1:
		diceReg|=0b0000000010000000 << 0;//DIS2
		break;
	case 2:
		diceReg|=0b0010000000000000 << 16;//DIS1
		diceReg|=0b0000000000000010 << 0;//DIS3
		break;
	case 3:
		diceReg|=0b0010000000000000 << 16;//DIS1
		diceReg|=0b0000000000000010 << 0;//DIS3
		diceReg|=0b0000000010000000 << 0;//DIS2
		break;
	case 4:
		diceReg|=0b0010000000000010 << 0;//DIS3
		diceReg|=0b0010000000000010 << 16;//DIS1
		break;
	case 5:
		diceReg|=0b0010000000000010 << 0;//DIS3
		diceReg|=0b0010000000000010 << 16;//DIS1
		diceReg|=0b0000000010000000 << 0;
		break;
	case 6:
		diceReg|=0b0010000000000010 << 0;//DIS3
		diceReg|=0b0010000000000010 << 16;//DIS1
		diceReg|=0b0000001000100000 << 0;//DIS2
		break;
	}
	sct_show(diceReg);
}
static const uint32_t reg_values[5][10] =
{
	{
		//PCDE--------GFAB @ DIS1
		0b0111000000000111 << 16,
		0b0100000000000001 << 16,
		0b0011000000001011 << 16,
		0b0110000000001011 << 16,
		0b0100000000001101 << 16,
		0b0110000000001110 << 16,
		0b0111000000001110 << 16,
		0b0100000000000011 << 16,
		0b0111000000001111 << 16,
		0b0110000000001111 << 16,
	},
	{
		//----PCDEGFAB---- @ DIS2
		0b0000011101110000 << 0,
		0b0000010000010000 << 0,
		0b0000001110110000 << 0,
		0b0000011010110000 << 0,
		0b0000010011010000 << 0,
		0b0000011011100000 << 0,
		0b0000011111100000 << 0,
		0b0000010000110000 << 0,
		0b0000011111110000 << 0,
		0b0000011011110000 << 0,
	},
	{
		//PCDE--------GFAB @ DIS3
		0b0111000000000111 << 0,
		0b0100000000000001 << 0,
		0b0011000000001011 << 0,
		0b0110000000001011 << 0,
		0b0100000000001101 << 0,
		0b0110000000001110 << 0,
		0b0111000000001110 << 0,
		0b0100000000000011 << 0,
		0b0111000000001111 << 0,
		0b0110000000001111 << 0,
	},
	{
		//----43215678---- << 16 @ LED
		0b0000000000000000 << 16,
		0b0000000100000000 << 16,
		0b0000001000000000 << 16,
		0b0000010000000000 << 16,
		0b0000100000000000 << 16,
		0b0000000010000000 << 16,
		0b0000000001000000 << 16,
		0b0000000000100000 << 16,
		0b0000000000010000 << 16,
		0b0000000000000000 << 16,
	},
	{
		//----43215678---- << 16 @ LED on
		0b0000000000000000 << 16,
		0b0000000100000000 << 16,
		0b0000001100000000 << 16,
		0b0000011100000000 << 16,
		0b0000111100000000 << 16,
		0b0000111110000000 << 16,
		0b0000111111000000 << 16,
		0b0000111111100000 << 16,
		0b0000111111110000 << 16,
		0b0000000000000000 << 16,
	}
};
