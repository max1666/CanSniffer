/*-- File description -------------------------------------------------------*/
/**
 *   @file:    usb_vcp.h
 *
 *   @author:  NIK El Team.
 *   @company: NIK El.
 */

#ifndef _USB_VCP_H
#define _USB_VCP_H
/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include "usbd_conf.h"
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
int8_t USB_VCP_ConfigCallback(uint8_t cmd, uint8_t* pbuf, uint16_t length, uint8_t interfaceNumber);
void USB_VCP_DataReceivedCallback(uint8_t* buffer, uint16_t length, uint8_t interfaceNumber);
void USB_VCP_SendData(uint8_t* buffer, uint16_t length, uint8_t interfaceNumber);
void USB_VCP_CableConnected(PCD_HandleTypeDef *hpcd);
void USB_VCP_CableDisconnected(PCD_HandleTypeDef *hpcd);
void USB_VCP_Run(void);

#endif // _USB_VCP_H
/*-- EOF --------------------------------------------------------------------*/
