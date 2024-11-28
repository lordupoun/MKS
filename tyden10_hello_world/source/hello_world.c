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
    uint32_t DWT1,DWT2;
    char password_stored[20]="1234";
    char input[20];
    uint8_t status;


    /* Init board hardware. */
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
#if !defined(DONT_ENABLE_FLASH_PREFETCH)
    /* enable flash prefetch for better performance */
    SYSCON->FMCCR |= SYSCON_FMCCR_PREFEN_MASK;
#endif

    PRINTF("hello world.\r\n");

    while (1)
    {
        //ch = GETCHAR();
        //PUTCHAR(ch);
    	//PRINTF("\r\nVlozte heslo: %d", DWT2-DWT1);
        //status=function(x,y);
    	PRINTF("\r\nVlozte heslo: ");
    	SCANF("%s",input);
        DWT1=DWT->CYCCNT;

        status=strcmp(input, password_stored);
        while(DWT->CYCCNT-DWT1<200)
        {
        	//wait, idealne neblokovane
        }

        DWT2=DWT->CYCCNT;
        PRINTF("\r\ninput: %s", input);
        if(status==0)
        {
        	PRINTF("\r\ninput correct");
        }
        else
        {
        	PRINTF("\r\ninput invalid");
        }
        PRINTF("\r\nCycles in function: %d", DWT2-DWT1);
    }
}
