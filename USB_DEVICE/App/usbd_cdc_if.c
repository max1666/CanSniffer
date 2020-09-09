/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v1.0_Cube
  * @brief          : Usb device for Virtual Com Port.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"
#include "usbd_conf.h"
#include "usbd_vcp.h"
/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */

/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
#if (NUM_OF_CDC_UARTS > 0)
uint8_t UsbCdcRxBuffer_1[APP_RX_DATA_1_SIZE];
#endif
#if (NUM_OF_CDC_UARTS > 1)
uint8_t UsbCdcRxBuffer_2[APP_RX_DATA_2_SIZE];
#endif
#if (NUM_OF_CDC_UARTS > 2)
uint8_t UsbCdcRxBuffer_3[APP_RX_DATA_3_SIZE];
#endif
#if (NUM_OF_CDC_UARTS > 3)
uint8_t UsbCdcRxBuffer_4[APP_RX_DATA_4_SIZE];
#endif

/** Data to send over USB CDC are stored in this buffer   */
#if (NUM_OF_CDC_UARTS > 0)
uint8_t UsbCdcTxBuffer_1[APP_TX_DATA_1_SIZE];
#endif
#if (NUM_OF_CDC_UARTS > 1)
uint8_t UsbCdcTxBuffer_2[APP_TX_DATA_2_SIZE];
#endif
#if (NUM_OF_CDC_UARTS > 2)
uint8_t UsbCdcTxBuffer_3[APP_TX_DATA_3_SIZE];
#endif
#if (NUM_OF_CDC_UARTS > 3)
uint8_t UsbCdcTxBuffer_4[APP_TX_DATA_4_SIZE];
#endif

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceHS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_HS(void);
static int8_t CDC_DeInit_HS(void);
static int8_t CDC_Control_HS(uint8_t cmd, uint8_t* pbuf, uint16_t length, uint8_t interfaceNumber);
static int8_t CDC_Receive_HS(uint8_t* pbuf, uint32_t *Len, uint8_t interfaceNumber);
static int8_t CDC_TransmitCplt_HS(uint8_t *pbuf, uint32_t *Len, uint8_t interfaceNumber);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_HS =
{
  CDC_Init_HS,
  CDC_DeInit_HS,
  CDC_Control_HS,
  CDC_Receive_HS,
  CDC_TransmitCplt_HS
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CDC media low layer over the USB HS IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_HS(void)
{
  /* USER CODE BEGIN 8 */
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_1, UsbCdcTxBuffer_1, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_1, UsbCdcRxBuffer_1);
  
#if (NUM_OF_CDC_UARTS > 0)
  USBD_CDC_SetTxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_1, UsbCdcTxBuffer_1, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_1, UsbCdcRxBuffer_1);
  USBD_CDC_ReceivePacket(&hUsbDeviceHS, CDC_ITF_NUMBER_1);
#endif
	
#if (NUM_OF_CDC_UARTS > 1)
  USBD_CDC_SetTxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_2, UsbCdcTxBuffer_2, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_2, UsbCdcRxBuffer_2);
  USBD_CDC_ReceivePacket(&hUsbDeviceHS, CDC_ITF_NUMBER_2);
#endif
	
#if (NUM_OF_CDC_UARTS > 2)
  USBD_CDC_SetTxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_3, UsbCdcTxBuffer_3, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_3, UsbCdcRxBuffer_3);
  USBD_CDC_ReceivePacket(&hUsbDeviceHS, CDC_ITF_NUMBER_3);
#endif
	
#if (NUM_OF_CDC_UARTS > 3)
  USBD_CDC_SetTxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_4, UsbCdcTxBuffer_4, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_4, UsbCdcRxBuffer_4);
  USBD_CDC_ReceivePacket(&hUsbDeviceHS, CDC_ITF_NUMBER_4);
#endif
  
  return (USBD_OK);
  /* USER CODE END 8 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @param  None
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_HS(void)
{
  /* USER CODE BEGIN 9 */
  return (USBD_OK);
  /* USER CODE END 9 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_HS(uint8_t cmd, uint8_t* pbuf, uint16_t length, uint8_t interfaceNumber)
{
  /* USER CODE BEGIN 10 */
  switch(cmd)
  {
  case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

  case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

  case CDC_SET_COMM_FEATURE:

    break;

  case CDC_GET_COMM_FEATURE:

    break;

  case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
  case CDC_SET_LINE_CODING:

    break;

  case CDC_GET_LINE_CODING:

    break;

  case CDC_SET_CONTROL_LINE_STATE:

    break;

  case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 10 */
}

/**
  * @brief Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAILL
  */
static int8_t CDC_Receive_HS(uint8_t* Buf, uint32_t *Len, uint8_t interfaceNumber)
{
  /* USER CODE BEGIN 11 */
  uint8_t result = 1;
  
  switch (interfaceNumber)
	{
#if (NUM_OF_CDC_UARTS > 0)
		case CDC_ITF_NUMBER_1:
		{
			USB_VCP_DataReceivedCallback(UsbCdcRxBuffer_1, *Len, CDC_ITF_NUMBER_1);
			
			USBD_CDC_SetRxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_1, UsbCdcRxBuffer_1);
			USBD_CDC_ReceivePacket(&hUsbDeviceHS, CDC_ITF_NUMBER_1);
		}
		break;
#endif
		
#if (NUM_OF_CDC_UARTS > 1)
		case CDC_ITF_NUMBER_2:
		{
			USB_VCP_DataReceivedCallback(UsbCdcRxBuffer_2, *Len, CDC_ITF_NUMBER_2);
			
			USBD_CDC_SetRxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_2, UsbCdcRxBuffer_2);
			USBD_CDC_ReceivePacket(&hUsbDeviceHS, CDC_ITF_NUMBER_2);
		}
		break;
#endif

#if (NUM_OF_CDC_UARTS > 2)
		case CDC_ITF_NUMBER_3:
		{
			USB_VCP_DataReceivedCallback(UsbCdcRxBuffer_3, *Len, CDC_ITF_NUMBER_3);
			
			USBD_CDC_SetRxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_3, UsbCdcRxBuffer_3);
			USBD_CDC_ReceivePacket(&hUsbDeviceHS, CDC_ITF_NUMBER_3);
		}
		break;
#endif

#if (NUM_OF_CDC_UARTS > 3)
		case CDC_ITF_NUMBER_4:
		{
			USB_VCP_DataReceivedCallback(UsbCdcRxBuffer_4, *Len, CDC_ITF_NUMBER_4);
			
			USBD_CDC_SetRxBuffer(&hUsbDeviceHS, CDC_ITF_NUMBER_4, UsbCdcRxBuffer_4);
			USBD_CDC_ReceivePacket(&hUsbDeviceHS, CDC_ITF_NUMBER_4);
		}
		break;
#endif
		
		default:
		{
			result = 0;
		}
		break;
	}
	
	if(result)
	{
		return (USBD_OK);
	}
	else
	{
		return (USBD_FAIL);
	}
  /* USER CODE END 11 */
}

/**
  * @brief  Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_HS(uint8_t* Buf, uint16_t Len, uint8_t interfaceNumber)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 12 */
  if(USBD_CDC_CheckSendingAvailable(&hUsbDeviceHS, interfaceNumber) == USBD_OK)
  {
    USBD_CDC_SetTxBuffer(&hUsbDeviceHS, interfaceNumber, Buf, Len);
    result = USBD_CDC_TransmitPacket(&hUsbDeviceHS, interfaceNumber);
  }
  /* USER CODE END 12 */
  return result;
}

/**
  * @brief  CDC_TransmitCplt_HS
  *         Data transmited callback
  *
  *         @note
  *         This function is IN transfer complete callback used to inform user that
  *         the submitted Data is successfully sent over USB.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_TransmitCplt_HS(uint8_t *Buf, uint32_t *Len, uint8_t interfaceNumber)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 14 */
  UNUSED(Buf);
  UNUSED(Len);
  UNUSED(interfaceNumber);
  /* USER CODE END 14 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
