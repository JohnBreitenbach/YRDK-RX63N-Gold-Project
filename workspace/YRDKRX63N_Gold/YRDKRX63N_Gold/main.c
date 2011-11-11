/*******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. 

* No other uses are authorized.
*
* This software is owned by Renesas Electronics Corp. and is protected under
* all applicable laws, including copyright laws.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
*
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name     : main.c
* Version       : 1.0
* Device(s)     : R5F563N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : Empty application project
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 22.Sep.2011     1.00        First release
*******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <machine.h>
#include "platform.h"
#include "yrdkrx62n_rspi_api.h"
#include "lcd.h"
#include "switches.h"

/*******************************************************************************
* Prototypes for local functions
*******************************************************************************/

/******************************************************************************
* Function name: main
* Description  : Main program function
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{
    /* Used to pace toggling of the LED */
    uint32_t led_counter;
    
    /* Initialize LCD */
    YRDKRX62N_RSPI_Init();
    lcd_initialize();
    
    /* Clear LCD */
    lcd_clear();
    
    /* Display message on LCD */
    lcd_display(LCD_LINE1, "  RENESAS   ");
    lcd_display(LCD_LINE2, " YRDKRX62N  ");

    /* 
        STDOUT is routed through the virtual console window tunneled through the JTAG debugger.
        Open the console window in HEW to see the output 
    */
    printf("This is the debug console\r\n");
    
    /* The three pushbuttons on the YRDK board are tied to interrupt lines, set them up here */
    switches_initialize();
    
    /* This is the main loop.  It does nothing but toggle LED4 periodically */
    while (1)
    {
        for (led_counter = 0; led_counter < 1000000; led_counter++)
        {
        }
        LED4 = ~LED4;
    }
}
