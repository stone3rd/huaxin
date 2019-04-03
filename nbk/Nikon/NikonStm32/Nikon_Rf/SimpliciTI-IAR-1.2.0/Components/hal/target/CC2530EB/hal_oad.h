/**************************************************************************************************
  Filename:       hal_oad.h
  Revised:        $Date:$
  Revision:       $Revision:$

  Description:    This module defines optionally-compiled Boot Code parameters for the CC2x3x.

  Copyright 2008-2009 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, 
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com. 
**************************************************************************************************/

#ifndef HAL_OAD_H
#define HAL_OAD_H

/*********************************************************************
 * INCLUDES
 */

#include "hal_board_cfg.h"
#include "hal_types.h"

/*********************************************************************
 * MACROS
 */

#if !defined HAL_OAD_BOOT_CODE
#define HAL_OAD_BOOT_CODE  FALSE
#endif

// MSP430 needs to explicitly pack OAD data structures - 8051 is automatic with byte alignment.
#define PACK_1

/*********************************************************************
 * CONSTANTS
 */

// Placement controlled by oad.xcl.
#define HAL_OAD_RC_START           0x2000//0x0800//0x1000//0x2000//0x2000//0x0800//0x5000//0x0800
#define HAL_OAD_CRC_ADDR           0x2888//0x0888//0x1088//0x0888
#define HAL_OAD_CRC_OSET          (HAL_OAD_CRC_ADDR - HAL_OAD_RC_START)

#define CPUID_VERI_ADDR             0x1FF0//0x1FF0//0x20188
#define CPUID_VERI_LEN			16

/* Note that corresponding changes must be made to oad.xcl when changing the source of Xtra-NV.
 * When using internal flash for XNV, (HAL_OAD_BOOT_PG_CNT + HAL_NV_PAGE_CNT) must be even.
 */
#define HAL_OAD_XNV_IS_INT         FALSE
#define HAL_OAD_XNV_IS_SPI        !HAL_OAD_XNV_IS_INT

/* The oad/oad-boot.xcl files only need 1 page of boot code (located on the first flash page),
 * but the Lock Bits Page is lost with OAD since it is not programmable.
 * So discard it here by faking that boot needs 2 pages.
 */
#define HAL_OAD_BOOT_PG_CNT        5///2  

/* To reduce the binary image size due to padding un-used code space, reduce HAL_OAD_DL_SIZE
 * to the minimum required for your Application and make the corresponding changes to oad_app.xcl.
 * This size must be an even multiple of HAL_FLASH_PAGE_SIZE.
 */
#if HAL_OAD_XNV_IS_SPI && !defined HAL_BOARD_CC2530EB_REV13
#define HAL_OAD_DL_MAX    0x40000
#define HAL_OAD_DL_SIZE  (0x40000 - ((HAL_NV_PAGE_CNT+HAL_OAD_BOOT_PG_CNT)*HAL_FLASH_PAGE_SIZE))
#define HAL_OAD_DL_OSET   0x0  // Configurable offset into an external NV.
#else
#define HAL_OAD_DL_MAX   (0x40000 - ((HAL_NV_PAGE_CNT+HAL_OAD_BOOT_PG_CNT)*HAL_FLASH_PAGE_SIZE))
#define HAL_OAD_DL_SIZE  (HAL_OAD_DL_MAX / 2)
#define HAL_OAD_DL_OSET  (HAL_OAD_DL_MAX / 2)
#endif

// To run OAD with the legacy ZOAD.exe PC tool, place the preamble in this legacy location.
#define PREAMBLE_OFFSET            0x8C


#define BOOT_FIRMWARE_VERSION		 0x00
#define AM1_IAP_WRITE_PAGE            0x41
#define AM1_IAP_ERASE_DEVICE	      0x42

#define AM1_IAP_CHECK_SUM		       0x44
#define AM1_IAP_READ_STATUS		0x45
#define AM1_IAP_SECURITYCHECK	       0x46
#define AM1_IAP_READ_64BYTE            0x47 

#define AM1_IAP_RST_POINT			0x48

/*********************************************************************
 * TYPEDEFS
 */

typedef enum {
  HAL_OAD_RC,  /* Run code / active image.          */
  HAL_OAD_DL   /* Downloaded code to be activated later. */
} image_t;

typedef struct {
	uint8 magic[2];
	uint32 len;
	uint16  vers;
	uint16  manu;
	uint16  prod;
} preamble_t;

#define	LDR_COMMUNICATION_BUFF_SIZE	 (80)

typedef struct
{ 
	unsigned char         com_number;                              // com number
	unsigned char         protocol_command;                       // protocol command
	unsigned char         no_used ;                           // frame data length
	unsigned char         data_length;                           // frame data length
	unsigned char         com_data[LDR_COMMUNICATION_BUFF_SIZE]; // data frame buffer
	unsigned char         msg_cks;                                  // frame checksum
 } Uart_message;

/*********************************************************************
 * FUNCTIONS
 */

uint8 HalOADChkDL(uint8 dlImagePreambleOffset);
void HalOADInvRC(void);
uint32 HalOADAvail(void);
void HalOADRead(uint32 oset, uint8 *pBuf, uint16 len, image_t type);
void HalOADWrite(uint32 oset, uint8 *pBuf, uint16 len, image_t type);
#endif