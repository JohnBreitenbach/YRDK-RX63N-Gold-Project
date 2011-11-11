/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : YRDKRX62N_RSPI_API.h
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : RX Family C Compiler
* H/W Platform : RDK+RX62N
* Description  : Board specific definitions and prototypes to access RSPI
* 
*******************************************************************************/
/*******************************************************************************
* History	   : 11.29.2010 Ver. 1.00 First Release
*******************************************************************************/
#ifndef YRDKRX62N_RSPI_API_HEADER_FILE
#define YRDKRX62N_RSPI_API_HEADER_FILE

#include <stdbool.h>

// enumeration for YRDKRX62N RSPI-0 chip selects
typedef enum {
    NO_DEVICE_SELECTED=0,
    SDMICRO_SELECTED,
    FLASH_SELECTED,
    WIFI_SELECTED,
    LCD_SELECTED
} device_selected ;


/******************************************************************************
Prototypes for the YRDKRX62N board specific APIs to share the RSPI hardware
******************************************************************************/
void YRDKRX62N_RSPI_Select(uint16_t chip_select);
void YRDKRX62N_RSPI_Deselect(uint16_t chip_select);
//void YRDKRX62N_RSPI_Init(uint8_t rspi_channel);
void YRDKRX62N_RSPI_Init(void);
bool YRDKRX62N_RSPI_Read(uint8_t *pDest, uint16_t usBytes);
bool YRDKRX62N_RSPI_Write(uint8_t *pSrc, uint16_t usBytes);
bool YRDKRX62N_RSPI_Lock(uint16_t pid);
bool YRDKRX62N_RSPI_Unlock(uint16_t pid);
void YRDKRX62N_RSPI_TxReady(void);

#endif /* YRDKRX62N_RSPI_API_HEADER_FILE */

