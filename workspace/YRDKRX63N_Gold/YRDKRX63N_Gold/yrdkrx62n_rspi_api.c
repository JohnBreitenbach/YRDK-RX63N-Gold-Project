/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : YRDKRX62N_RSPI_API.C
* Version       : 1.00
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Functions for using RSPI devices on the YRDKRX62N
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 02.Dec.2010     1.00        First release
*******************************************************************************/

/**********************************************************************************
System Includes
***********************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef NULL
#define NULL	0
#endif

/**********************************************************************************
User Includes
***********************************************************************************/

/* Defines for the YRDKRX62N hardware */
//#include "YRDKRX62N.h"

/* Defines for YRDKRX62N RSPI support */
#include "YRDKRX62N_RSPI_API.h"

//#include "iodefine_rx62n.h"
#include "platform.h"

#if PLATFORM_RDK62N==1
    #define RSPI_DATA_REG   RSPI0.SPDR.LONG
#elif PLATFORM_RDK63N==1
    #define RSPI_DATA_REG   RSPI0.SPDR
#else
    #error INVALID PLATFORM
#endif    

static uint16_t YRDKRX62N_RSPI_Access_Locked_By = NO_DEVICE_SELECTED; // use spi_cs enum for pids

/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Select
* Description  : Enable device chip select on SPI channel
* Arguments    : uint16_t SPI chip_select defined in rdkrx62n.h
* Return Value : none
*””FUNC COMMENT END””*********************************************************/
void YRDKRX62N_RSPI_Select(uint16_t chip_select)
{
    switch ( chip_select)
    {
    case SDMICRO_SELECTED:          /* Enable the SD card */
        SDMICRO_CS = 0 ;
        break;
    case FLASH_SELECTED:            /* Enable Micron flash */
        FLASH_CS = 0 ;    
        break;
    case WIFI_SELECTED:             /* Enable Redpine WiFi card */
        WIFI_CS = 0 ;
        break;
    case LCD_SELECTED:              /* Enable Okaya display */
        LCD_CS = 0 ;
        break;
    default:
        return;
    }     
}

/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Deselect
* Description  : Disables device chip select
* Arguments    : uint16_t SPI chip_select defined in rdkrx62n.h
* Return Value : none
*””FUNC COMMENT END””*********************************************************/
void YRDKRX62N_RSPI_Deselect(uint16_t chip_select)
{
    switch ( chip_select)
    {
    case SDMICRO_SELECTED:          /* Disable the SD card */
        SDMICRO_CS = 1 ;
        break;
    case FLASH_SELECTED:            /* Disable the Micron flash */
        FLASH_CS = 1 ;
        break;
    case WIFI_SELECTED:             /* Disable the Redpine WiFi card */
        WIFI_CS = 1 ;
        break;
    case LCD_SELECTED:              /* Disable the Okaya display */
        LCD_CS = 1 ;
        break;
    default:
        return;
    }     
}

/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Init
* Description  : Initializes SPI channel
* Arguments    : none
* Return Value : none
*””FUNC COMMENT END””*********************************************************/
void YRDKRX62N_RSPI_Init(void)
{
    /* Init RSPI channel 0 */

    /* Initialize SPI channel 0 in SPI master mode, on the 'A' pins,  
       keep all SSLs disabled */       
    
    /* Turn on RSPI0 */
    MSTP(RSPI0) = 0;

    /* Set pin control register (SPPCR) */
    RSPI0.SPPCR.BYTE = 0x00;

    /* Set RSPI bit rate (SPBR) */
    /* -Set baud rate to 8Mbps (48MHz / (2 * (2 + 1) * 2^0) ) = 8Mbps */
    RSPI0.SPBR.BYTE = 127;        // TODO: Fix this (was 2)

    /* Set RSPI data control register (SPDCR) */
    /* -SPDR is accessed in longwords (32 bits) 
       -Transfer 1 frame at a time */
    RSPI0.SPDCR.BYTE = 0x20;

    /* Set RSPI clock delay registers (SPCKD) */
    //RSPI0.SPCKD.BYTE = 7;

    /* Set RSPI slave select negation delay register (SSLND) */
    //RSPI0.SSLND.BYTE = 7;

    /* Set RSPI next-access dealy register (SPND) */
    //RSPI0.SPND.BYTE = 7;

    /* Set RSPI control register 2 (SPCR2) */
    /* -Disable Idle interrupt */
    RSPI0.SPCR2.BYTE = 0x00;

    /* Set RSPI command register 0 (SPCMD0) */
    /* -MSB first
       -8 bits data length
       -SSL0 (handled manually)
       -Use bit rate % 1       
        */       
    RSPI0.SPCMD0.WORD = 0x0400;    
    
    /* Setup ICU for RSPI */
    /* Set shared IPL for RSPI0 */
    IPR(RSPI0, SPRI0) = 3;
    /* Disable receive buffer full interrupt */
    IEN(RSPI0, SPRI0) = 0;
    /* Disable transmit buffer empty interrupt */
    IEN(RSPI0, SPTI0) = 0;
    /* Enable RSPI idle interrupt */
    //IEN(RSPI0, SPII0) = 1;

    /* Clear pending receive buffer full interrupts */
    IR(RSPI0, SPRI0) = 0;

#if PLATFORM_RDK62N==1
    /* Set input/output ports */
    /* Choose A pin group and enable SPI pins */    
    IOPORT.PFGSPI.BYTE = 0x0E;
    /* RSPCKA-A is output */
    PORTC.DDR.BIT.B5 = 1;
    /* MOSIA-A is ouptut */
    PORTC.ICR.BIT.B6 = 1;
    PORTC.DDR.BIT.B6 = 0;
    /* MISOA-A is input */
    PORTC.ICR.BIT.B7 = 1;
    PORTC.DDR.BIT.B7 = 0;
    /* SSLA1-A is output (manually) */
    PORTC.DDR.BIT.B0 = 1;
#elif PLATFORM_RDK63N==1

#if 0
    /* Port C -  SPI signals, chip selects, peripheral reset */
    PORTC.PODR.BYTE = 0x00 ;    /* */
    PORTC.PMR.BYTE  = 0x00 ;    /* All GPIO for now */
    MPC.PC5PFS.BYTE = 0x0D ;    /* PC5 is RSPCKA */
    MPC.PC6PFS.BYTE = 0x0D ;    /* PC6 is MOSIA */
    MPC.PC7PFS.BYTE = 0x0D ;    /* PC7 is MISOA */
    PORTC.PMR.BYTE  = 0xE0 ;    /* PC5-7 assigned to SPI peripheral */
    PORTC.PODR.BYTE = 0x17 ;    /* Chip selects high to start */
    PORTC.PDR.BYTE  = 0x7F ;    /* All outputs except MISO */
#endif

#else
    #error PLATFORM MUST BE RDK RX62N OR RDK RX63N TO USE THIS MODULE!
#endif
    
    /* Disable slave selects */
    SDMICRO_CS = 1 ;
    FLASH_CS = 1 ;
    WIFI_CS = 1 ;
    LCD_CS = 1 ;

    /* Set RSPI control register (SPCR) */        
    /* -Clock synchronous operation (3-wire)
       -Full duplex operation
       -Master mode
       -SPTI and SPRI enabled in RSPI (have to check ICU also)
       -Enable RSPI function */
    RSPI0.SPCR.BYTE = 0xE9;        
  
}

/*””FUNC COMMENT””*************************************************************
* Function Name: SPI_send_receive
* Description  : Performs SPI transfers
* Arguments    : pSrc -  pointer to data buffer with data to be transmitted.
*                        If NULL, const 0xFF as source.
*                pDest - pointer to location to put the received data (can be
*                        same as pSrc if desired). 
*                        If NULL, receive data discarded.
*                usBytes - number of bytes to be sent/received
* Return Value : none
*””FUNC COMMENT END””*********************************************************/
void SPI_send_receive(uint8_t const *pSrc, uint8_t *pDest, uint16_t usBytes )
{    	
    uint16_t i;  
    volatile uint32_t temp;

    for (i = 0; i < usBytes; i++)
    {
        /* Ensure transmit register is empty */
        while (RSPI0.SPSR.BIT.IDLNF) ;
        
        /* If just reading then transmit 0xFF */
#if PLATFORM_RDK62N==1        
        RSPI0.SPDR.LONG = (pSrc == NULL) ? 0xFF : pSrc[i];
#elif PLATFORM_RDK63N==1
        RSPI0.SPDR = (pSrc == NULL) ? 0xFFFFFFFF : pSrc[i] ;
#else
    #error INVALID PLATFORM!
#endif            
        
        /* Wait for transfer to complete */
        //while (RSPI0.SPSR.BIT.IDLNF) ;
        
        /* Transfer is complete when a byte has been shifted in (full duplex) */
        while (IR(RSPI0, SPRI0) == 0);
        
        /* Read received data.  If transmit only, then ignore it */
        if (pDest == NULL)
        {
#if PLATFORM_RDK62N==1            
            temp = RSPI0.SPDR.LONG;
#elif PLATFORM_RDK63N==1
            temp = RSPI0.SPDR ;
            temp >>= 16 ;
#else
    #error INVALID PLATFORM!
#endif
        }
        else
        {
#if PLATFORM_RDK62N==1            
            pDest[i] = (unsigned char) (RSPI0.SPDR.LONG & 0xFF);
#elif PLATFORM_RDK63N==1
            temp = RSPI0.SPDR ;
            temp >>= 16 ;
            pDest[i] = (unsigned char) (temp & 0xFF);
#else
    #error INVALID PLATFORM!
#endif
        }

        /* Clear pending read interrupts */
        IR(RSPI0, SPRI0) = 0;
        
    }
}

/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Read
* Description  : Performs SPI transfers
* Arguments    : pDest - Pointer to location to put the received data 
*                        Returned value will be incremented by number
*                        of bytes received.
*                usBytes - number of bytes to be received
* Return Value : bool     true, success
*                       false, failure
*””FUNC COMMENT END””*********************************************************/
bool YRDKRX62N_RSPI_Read(uint8_t *pDest, uint16_t usBytes )
{
    uint16_t i;  
    volatile uint32_t temp;
    bool ret = true ;

    for (i = 0; i < usBytes; i++)
    {
        /* Ensure transmit register is empty */
        while (RSPI0.SPSR.BIT.IDLNF) ;
        
        /* If just reading then transmit 0xFF */
#if PLATFORM_RDK62N==1            
        RSPI0.SPDR.LONG = 0xFFFFFFFF ;
#elif PLATFORM_RDK63N==1
        RSPI0.SPDR = 0xFFFFFFFF ;
#else
    #error INVALID PLATFORM!
#endif
        
        /* Wait for transfer to complete */
        //while (RSPI0.SPSR.BIT.IDLNF) ;
        /* Transfer is complete when a byte has been shifted in (full duplex) */
        while (IR(RSPI0, SPRI0) == 0);
        
        
        /* Read received data.  If transmit only, then ignore it */
        if (pDest == NULL)
        {
#if PLATFORM_RDK62N==1            
            temp = RSPI0.SPDR.LONG;
#elif PLATFORM_RDK63N==1
            temp = RSPI0.SPDR;
#else
    #error INVALID PLATFORM!
#endif
        }
        else
        {
#if PLATFORM_RDK62N==1            
            pDest[i] = (unsigned char) (RSPI0.SPDR.LONG & 0xFF);
#elif PLATFORM_RDK63N==1
            temp = RSPI0.SPDR ;
            temp >>= 16 ;
            pDest[i] = (unsigned char) (temp & 0xFF);
#else
    #error INVALID PLATFORM!
#endif
        }
        
        /* Clear any pending read interrupts */
        IR(RSPI0, SPRI0) = 0 ;
    }
  
    return ret;
}
/******************************************************************************
End of function YRDKRX62N_RSPI_Read
******************************************************************************/

/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Write
* Description  : Write to an SPI device
* Arguments    : pSrc -  Pointer to data buffer with data to be transmitted.
*                        Returned value will be incremented by number of attempted
*                        writes.
*                usBytes - number of bytes to be sent
* Return Value : bool     true, success
*                       false, failure
*””FUNC COMMENT END””*********************************************************/
bool YRDKRX62N_RSPI_Write(uint8_t *pSrc, uint16_t usBytes )
{
  
    bool ret = true;
    uint16_t i;  
    volatile uint32_t temp;
    uint8_t *myPtr = pSrc ;

    for (i = 0; i < usBytes; i++)
    {
        /* Ensure transmit register is empty */
        while (RSPI0.SPSR.BIT.IDLNF) ;
        
        /* If just reading then transmit 0xFF */
#if PLATFORM_RDK62N==1            
        RSPI0.SPDR.LONG = (pSrc == NULL) ? 0xFF : pSrc[i];
#elif PLATFORM_RDK63N==1
        RSPI0.SPDR = (pSrc == NULL) ? 0xFFFFFFFF : (uint32_t) *myPtr++ ;
#else
    #error INVALID PLATFORM!
#endif
        
        /* Wait for transfer to complete */
        //while (RSPI0.SPSR.BIT.IDLNF) ;
        /* Transfer is complete when a byte has been shifted in (full duplex) */
        while (IR(RSPI0, SPRI0) == 0);
        
        /* Read received data.  If transmit only, then ignore it */
#if PLATFORM_RDK62N==1            
        temp = RSPI0.SPDR.LONG;
#elif PLATFORM_RDK63N==1
        temp = RSPI0.SPDR ;
#else
    #error INVALID PLATFORM!
#endif

        /* Clear pending interrupts */
        IR(RSPI0, SPRI0) = 0 ;
    }
    return ret;
}

/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Lock
* Description  : Get lock on RSPI access
* Arguments    : uint16_t    pid - unique program ID
* Return Value : bool - true, lock acquired
*                       false, lock not acquired
*””FUNC COMMENT END””*********************************************************/
bool YRDKRX62N_RSPI_Lock(uint16_t pid)
{
    bool ret = true;

    if ( YRDKRX62N_RSPI_Access_Locked_By == NO_DEVICE_SELECTED )
    {
        YRDKRX62N_RSPI_Access_Locked_By = pid;
    }
    else
    {
        ret = false;
    }

    return ret;


}

/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Unlock
* Description  : Unlock RSPI access
* Arguments    : uint16_t pid -- pid that locked access.
* Return Value : true -- correct pid
*                false -- wrong pid     
*””FUNC COMMENT END””*********************************************************/
bool YRDKRX62N_RSPI_Unlock(uint16_t pid)
{
    bool ret = true;

    if ( YRDKRX62N_RSPI_Access_Locked_By == pid )
    {
        YRDKRX62N_RSPI_Access_Locked_By = NO_DEVICE_SELECTED;
    }
    else
    {
        ret = false;
    }
    return ret;
}

/*””FUNC COMMENT””*************************************************************
* Function Name: YRDKRX62N_RSPI_Tx_Ready
* Description  : Make sure the transmit register is empty, blocks until it is.
* Arguments    : none
* Return Value : none
*””FUNC COMMENT END””*********************************************************/
void YRDKRX62N_RSPI_TxReady(void)
{
    while (RSPI0.SPSR.BIT.IDLNF) ;
}
