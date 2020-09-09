/**
  ******************************************************************************
  * @file    usbd_cdc.h
  * @author  MCD Application Team
  * @brief   header file for the usbd_cdc.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CDC_H
#define __USB_CDC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup usbd_cdc
  * @brief This file is the Header file for usbd_cdc.c
  * @{
  */


/** @defgroup usbd_cdc_Exported_Defines
  * @{
  */
//#define CDC_IN_EP                                   0x81U  /* EP1 for data IN */
//#define CDC_OUT_EP                                  0x01U  /* EP1 for data OUT */
//#define CDC_CMD_EP                                  0x82U  /* EP2 for CDC commands */

#if (NUM_OF_CDC_UARTS > 0)
#define CDC_ITF_NUMBER_1                                1  // ITF number
#define CDC_ITF_CMD_1                               0x00U  // ITF1 for data IN
#define CDC_ITF_DATA_1                              0x01U  // ITF1 for data OUT
#define CDC_EP_DATA_OUT_1                           0x01U  // EP1 for data OUT
#define CDC_EP_DATA_IN_1                            0x81U  // EP1 for data IN
#define CDC_EP_CMD_1                                0x82U  // EP2 for CDC commands
#endif

#if (NUM_OF_CDC_UARTS > 1)
#define CDC_ITF_NUMBER_2                                2  // ITF number
#define CDC_ITF_CMD_2                               0x02U  // ITF2 for data IN
#define CDC_ITF_DATA_2                              0x03U  // ITF2 for data OUT
#define CDC_EP_DATA_OUT_2                           0x03U  // EP3 for data OUT
#define CDC_EP_DATA_IN_2                            0x83U  // EP3 for data IN
#define CDC_EP_CMD_2                                0x84U  // EP4 for CDC commands
#endif

#if (NUM_OF_CDC_UARTS > 2)
#define CDC_ITF_NUMBER_3                                3  // ITF number
#define CDC_ITF_CMD_3                               0x04U  // ITF3 for data IN
#define CDC_ITF_DATA_3                              0x05U  // ITF3 for data OUT
#define CDC_EP_DATA_OUT_3                           0x05U  // EP5 for data OUT
#define CDC_EP_DATA_IN_3                            0x85U  // EP5 for data IN
#define CDC_EP_CMD_3                                0x86U  // EP6 for CDC commands
#endif

#if (NUM_OF_CDC_UARTS > 3)
#define CDC_ITF_NUMBER_4                                4  // ITF number
#define CDC_ITF_CMD_4                               0x06U  // ITF4 for data IN
#define CDC_ITF_DATA_4                              0x07U  // ITF4 for data OUT
#define CDC_EP_DATA_OUT_4                           0x07U  // EP7 for data OUT
#define CDC_EP_DATA_IN_4                            0x87U  // EP7 for data IN
#define CDC_EP_CMD_4                                0x88U  // EP8 for CDC commands
#endif

#ifndef CDC_HS_BINTERVAL
#define CDC_HS_BINTERVAL                            0x10U
#endif /* CDC_HS_BINTERVAL */

#ifndef CDC_FS_BINTERVAL
#define CDC_FS_BINTERVAL                            0x10U
#endif /* CDC_FS_BINTERVAL */

/* CDC Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define CDC_DATA_HS_MAX_PACKET_SIZE                 64U  /* Endpoint IN & OUT Packet size */
#define CDC_DATA_FS_MAX_PACKET_SIZE                 64U  /* Endpoint IN & OUT Packet size */
#define CDC_CMD_PACKET_SIZE                         8U  /* Control Endpoint Packet size */

//#define USB_CDC_CONFIG_DESC_SIZ                     67U
#define CDC_DATA_HS_IN_PACKET_SIZE                  CDC_DATA_HS_MAX_PACKET_SIZE
#define CDC_DATA_HS_OUT_PACKET_SIZE                 CDC_DATA_HS_MAX_PACKET_SIZE

#define CDC_DATA_FS_IN_PACKET_SIZE                  CDC_DATA_FS_MAX_PACKET_SIZE
#define CDC_DATA_FS_OUT_PACKET_SIZE                 CDC_DATA_FS_MAX_PACKET_SIZE

/*---------------------------------------------------------------------*/
/*  CDC definitions                                                    */
/*---------------------------------------------------------------------*/
#define CDC_SEND_ENCAPSULATED_COMMAND               0x00U
#define CDC_GET_ENCAPSULATED_RESPONSE               0x01U
#define CDC_SET_COMM_FEATURE                        0x02U
#define CDC_GET_COMM_FEATURE                        0x03U
#define CDC_CLEAR_COMM_FEATURE                      0x04U
#define CDC_SET_LINE_CODING                         0x20U
#define CDC_GET_LINE_CODING                         0x21U
#define CDC_SET_CONTROL_LINE_STATE                  0x22U
#define CDC_SEND_BREAK                              0x23U

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

/**
  * @}
  */
typedef struct
{
  uint32_t bitrate;
  uint8_t  format;
  uint8_t  paritytype;
  uint8_t  datatype;
} USBD_CDC_LineCodingTypeDef;

typedef struct _USBD_CDC_Itf
{
  int8_t (* Init)(void);
  int8_t (* DeInit)(void);
  int8_t (* Control)(uint8_t cmd, uint8_t *pbuf, uint16_t length, uint8_t interfaceNumber);
  int8_t (* Receive)(uint8_t *Buf, uint32_t *Len, uint8_t interfaceNumber);
  int8_t (* TransmitCplt)(uint8_t *Buf, uint32_t *Len, uint8_t interfaceNumber);
} USBD_CDC_ItfTypeDef;


typedef struct
{
#if defined (USE_OTG_FS)
  uint32_t data[CDC_DATA_FS_MAX_PACKET_SIZE / 4U];      /* Force 32bits alignment */
#endif
#if defined (USE_OTG_HS)
  uint32_t data[CDC_DATA_HS_MAX_PACKET_SIZE / 4U];      /* Force 32bits alignment */
#endif
  uint8_t  CmdOpCode;
  uint8_t  CmdLength;
  uint8_t  *RxBuffer;
  uint8_t  *TxBuffer;
  uint32_t RxLength;
  uint32_t TxLength;

  __IO uint32_t TxState;
  __IO uint32_t RxState;
} USBD_CDC_HandleTypeDef;


typedef struct
{
	uint8_t interfaceNumber;
	uint8_t data_in_ep;
	uint8_t data_out_ep;
	uint8_t command_ep;
	uint8_t command_itf;
} USBD_CDC_ParamsTypeDef;


/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef USBD_CDC;
#define USBD_CDC_CLASS &USBD_CDC
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t USBD_CDC_CheckSendingAvailable(USBD_HandleTypeDef *pdev, uint8_t interfaceNumber);
uint8_t USBD_CDC_RegisterInterface(USBD_HandleTypeDef *pdev, USBD_CDC_ItfTypeDef *fops);
uint8_t USBD_CDC_SetTxBuffer(USBD_HandleTypeDef *pdev, uint8_t interfaceNumber, uint8_t *pbuff, uint32_t length);
uint8_t USBD_CDC_SetRxBuffer(USBD_HandleTypeDef *pdev, uint8_t interfaceNumber, uint8_t *pbuff);
uint8_t USBD_CDC_ReceivePacket(USBD_HandleTypeDef *pdev, uint8_t interfaceNumber);
uint8_t USBD_CDC_TransmitPacket(USBD_HandleTypeDef *pdev, uint8_t interfaceNumber);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_CDC_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
