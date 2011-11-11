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
* File Name     : Platform.h
* Version       : 1.0
* Device(s)     : R5F563N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : Defines which platform is being built
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 22.Sep.2011     1.00        First release
*******************************************************************************/

/******************************************************************************
 Include files
******************************************************************************/

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

/* 
    You can add a "PLATFORM_xxxx" macro for your customer HW platform here 
    You'll need to include the proper iodefine.h file below and a header file
    with board-specific defintions.
    The "PLATFORM_xxxx" macro is defined under the Build options dialog in HEW.
*/
#ifndef PLATFORM_RDK63N
    #error NO PLATFORM DEFINED!
#endif

#if PLATFORM_RDK63N==1
    #include "iodefine_rx63n.h"
    #include "yrdkrx63n.h"
#else
    #error INVALID PLATFORM!
#endif

#endif
