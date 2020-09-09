/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : App/usbd_desc.c
  * @version        : v1.0_Cube
  * @brief          : This file implements the USB device descriptors.
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
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_conf.h"

#include "usbhelper.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @addtogroup USBD_DESC
  * @{
  */

/** @defgroup USBD_DESC_Private_TypesDefinitions USBD_DESC_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_DESC_Private_Defines USBD_DESC_Private_Defines
  * @brief Private defines.
  * @{
  */

#define USBD_VID                           1172
#define USBD_PID                           4097

//#define USBD_VID                           1155
//#define USBD_PID                           22336

#define USBD_LANGID_STRING                 1033
#define USBD_MANUFACTURER_STRING           "Lab"
#define USBD_PRODUCT_STRING                "CanSniffer Interface"
#define USBD_CONFIGURATION_STRING          "CDC Config"
#define USBD_INTERFACE_STRING              "CDC Interface"

#define USB_SIZ_BOS_DESC                   0x0C

/* USER CODE BEGIN PRIVATE_DEFINES */
#define USBD_PRODUCT_STRING_IF1           "CanSniffer Interface 1"
#define USBD_PRODUCT_STRING_IF2           "CanSniffer Interface 2"
#define USBD_PRODUCT_STRING_IF3           "CanSniffer Interface 3"
#define USBD_PRODUCT_STRING_IF4           "CanSniffer Interface 4"
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/** @defgroup USBD_DESC_Private_Macros USBD_DESC_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_DESC_Private_FunctionPrototypes USBD_DESC_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static void Get_SerialNum(void);
static void IntToUnicode(uint32_t value, uint8_t * pbuf, uint8_t len);

/**
  * @}
  */

/** @defgroup USBD_DESC_Private_FunctionPrototypes USBD_DESC_Private_FunctionPrototypes
  * @brief Private functions declaration for HS.
  * @{
  */

uint8_t * USBD_Dev_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t * USBD_Dev_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t * USBD_Dev_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t * USBD_Dev_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length, uint16_t wLength);
uint8_t * USBD_Dev_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t * USBD_Dev_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t * USBD_Dev_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);

#if (USBD_LPM_ENABLED == 1)
uint8_t * USBD_HS_USR_BOSDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
#endif /* (USBD_LPM_ENABLED == 1) */

/**
  * @}
  */

/** @defgroup USBD_DESC_Private_Variables USBD_DESC_Private_Variables
  * @brief Private variables.
  * @{
  */

USBD_DescriptorsTypeDef Dev_Desc =
{
  USBD_Dev_DeviceDescriptor
, USBD_Dev_LangIDStrDescriptor
, USBD_Dev_ManufacturerStrDescriptor
, USBD_Dev_ProductStrDescriptor
, USBD_Dev_SerialStrDescriptor
, USBD_Dev_ConfigStrDescriptor
, USBD_Dev_InterfaceStrDescriptor
#if (USBD_LPM_ENABLED == 1)
, USBD_HS_USR_BOSDescriptor
#endif /* (USBD_LPM_ENABLED == 1) */
};

#if defined ( __ICCARM__ ) /* IAR Compiler */
  #pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
/* USB Standard Device Descriptor */
static const struct device_descriptor USBD_Dev_DeviceDesc =
{
  sizeof(USBD_Dev_DeviceDesc),    /* bLength */
  USB_DESC_TYPE_DEVICE,       /* bDescriptorType */
#if (USBD_LPM_ENABLED == 1)
  0x01,                       /*bcdUSB */ /* changed to USB version 2.01
                                             in order to support LPM L1 suspend
                                             resume test of USBCV3.0*/
#else
  0x00,                       /*bcdUSB */
#endif /* (USBD_LPM_ENABLED == 1) */
  0x02,
  //USB_UINT16(0x0200),         /* bcdUSB */
  
  0x00,                       /* bDeviceClass */
  0x00,                       /* bDeviceSubClass */
  0x00,                       /* bDeviceProtocol */
//  0xEF,                       /* bDeviceClass */
//  0x02,                       /* bDeviceSubClass */
//  0x01,                       /* bDeviceProtocol */
  USB_MAX_EP0_SIZE,           /* bMaxPacketSize */

  LOBYTE(USBD_VID),           /*idVendor*/
  HIBYTE(USBD_VID),           /*idVendor*/
  LOBYTE(USBD_PID),           /*idProduct*/
  HIBYTE(USBD_PID),           /*idProduct*/
  //USB_UINT16(USBD_VID),       /* idVendor */
  //USB_UINT16(USBD_PID),       /* idProduct */
  
  0x00,                       /*bcdDevice rel. 2.00*/
  0x02,
  //USB_UINT16(0x0200),         /* bcdDevice */
  USBD_IDX_MFC_STR,           /* Index of manufacturer string */
  USBD_IDX_PRODUCT_STR,       /* Index of product string */
  USBD_IDX_SERIAL_STR,        /* Index of serial number string */
  USBD_MAX_NUM_CONFIGURATION  /* bNumConfigurations */
};

/** BOS descriptor. */
#if (USBD_LPM_ENABLED == 1)
#if defined ( __ICCARM__ ) /* IAR Compiler */
  #pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN uint8_t USBD_Dev_BOSDesc[USB_SIZ_BOS_DESC] __ALIGN_END =
{
  0x5,
  USB_DESC_TYPE_BOS,
  0xC,
  0x0,
  0x1,  /* 1 device capability */
        /* device capability */
  0x7,
  USB_DEVICE_CAPABITY_TYPE,
  0x2,
  0x2,  /*LPM capability bit set */
  0x0,
  0x0,
  0x0
};
#endif /* (USBD_LPM_ENABLED == 1) */

/**
  * @}
  */

/** @defgroup USBD_DESC_Private_Variables USBD_DESC_Private_Variables
  * @brief Private variables.
  * @{
  */

#if defined ( __ICCARM__ ) /* IAR Compiler */
  #pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */

/** USB lang indentifier descriptor. */
__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END =
{
     USB_LEN_LANGID_STR_DESC,
     USB_DESC_TYPE_STRING,
     LOBYTE(USBD_LANGID_STRING),
     HIBYTE(USBD_LANGID_STRING)
};

#if defined ( __ICCARM__ ) /* IAR Compiler */
  #pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
/* Internal string descriptor. */
__ALIGN_BEGIN uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t USBD_StringSerial[USB_SIZ_STRING_SERIAL] __ALIGN_END = {
  USB_SIZ_STRING_SERIAL,
  USB_DESC_TYPE_STRING,
};

/**
  * @}
  */

/** @defgroup USBD_DESC_Private_Functions USBD_DESC_Private_Functions
  * @brief Private functions.
  * @{
  */

/**
  * @brief  Return the device descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t * USBD_Dev_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  *length = sizeof(USBD_Dev_DeviceDesc);
  return (uint8_t *)&USBD_Dev_DeviceDesc;
}

/**
  * @brief  Return the LangID string descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t * USBD_Dev_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  *length = sizeof(USBD_LangIDDesc);
  return USBD_LangIDDesc;
}

/**
  * @brief  Return the product string descriptor
  * @param  speed : current device speed
  * @param  length : pointer to data length variable
  * @retval pointer to descriptor buffer
  */
uint8_t * USBD_Dev_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length, uint16_t wLength)
{
  if(wLength == 0x00FF)
	{
		USBD_GetString((uint8_t *)USBD_PRODUCT_STRING, USBD_StrDesc, length);
	}
	else
	{
		static uint8_t interfaceNumber = 0;
		static uint16_t descLength = 0;
		
		switch (interfaceNumber)
		{
#if (NUM_OF_CDC_UARTS > 0)
			case 0:
			{
				USBD_GetString((uint8_t *)USBD_PRODUCT_STRING_IF1, USBD_StrDesc, &descLength);
				
				if(wLength == 0x0004)
				{
					*length = descLength;
				}
				else if(wLength == descLength)
				{
					*length = descLength;
					
					descLength = 0;
					interfaceNumber++;
					
					if(interfaceNumber == NUM_OF_CDC_UARTS)
					{
						interfaceNumber = 0;
					}
				}
			}
			break;
#endif
      
#if (NUM_OF_CDC_UARTS > 1)
			case 1:
			{
				USBD_GetString((uint8_t *)USBD_PRODUCT_STRING_IF2, USBD_StrDesc, &descLength);
				
				if(wLength == 0x0004)
				{
					*length = descLength;
				}
				else if(wLength == descLength)
				{
					*length = descLength;
					
					descLength = 0;
					interfaceNumber++;
					
					if(interfaceNumber == NUM_OF_CDC_UARTS)
					{
						interfaceNumber = 0;
					}
				}
			}
			break;
#endif

#if (NUM_OF_CDC_UARTS > 2)
			case 2:
			{
				USBD_GetString((uint8_t *)USBD_PRODUCT_STRING_IF3, USBD_StrDesc, &descLength);
				
				if(wLength == 0x0004)
				{
					*length = descLength;
				}
				else if(wLength == descLength)
				{
					*length = descLength;
					
					descLength = 0;
					interfaceNumber++;
					
					if(interfaceNumber == NUM_OF_CDC_UARTS)
					{
						interfaceNumber = 0;
					}
				}
			}
			break;
#endif

#if (NUM_OF_CDC_UARTS > 3)
			case 2:
			{
				USBD_GetString((uint8_t *)USBD_PRODUCT_STRING_IF4, USBD_StrDesc, &descLength);
				
				if(wLength == 0x0004)
				{
					*length = descLength;
				}
				else if(wLength == descLength)
				{
					*length = descLength;
					
					descLength = 0;
					interfaceNumber++;
					
					if(interfaceNumber == NUM_OF_CDC_UARTS)
					{
						interfaceNumber = 0;
					}
				}
			}
			break;
#endif

      default:
      {
        interfaceNumber = 0;
        descLength = 0;
      }
      break;
		}
	}
//  if(speed == USBD_SPEED_HIGH)
//  {
//    USBD_GetString((uint8_t *)USBD_PRODUCT_STRING, USBD_StrDesc, length);
//  }
//  else
//  {
//    USBD_GetString((uint8_t *)USBD_PRODUCT_STRING, USBD_StrDesc, length);
//  }

  return USBD_StrDesc;
}

/**
  * @brief  Return the manufacturer string descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t * USBD_Dev_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  USBD_GetString((uint8_t *)USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

/**
  * @brief  Return the serial number string descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t * USBD_Dev_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  *length = USB_SIZ_STRING_SERIAL;

  /* Update the serial number string descriptor with the data from the unique
   * ID */
  Get_SerialNum();
  /* USER CODE BEGIN USBD_HS_SerialStrDescriptor */

  /* USER CODE END USBD_HS_SerialStrDescriptor */

  return (uint8_t *) USBD_StringSerial;
}

/**
  * @brief  Return the configuration string descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t * USBD_Dev_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if(speed == USBD_SPEED_HIGH)
  {
    USBD_GetString((uint8_t *)USBD_CONFIGURATION_STRING, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_CONFIGURATION_STRING, USBD_StrDesc, length);
  }
  return USBD_StrDesc;
}

/**
  * @brief  Return the interface string descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t * USBD_Dev_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if(speed == 0)
  {
    USBD_GetString((uint8_t *)USBD_INTERFACE_STRING, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_INTERFACE_STRING, USBD_StrDesc, length);
  }
  return USBD_StrDesc;
}

#if (USBD_LPM_ENABLED == 1)
/**
  * @brief  Return the BOS descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t * USBD_Dev_USR_BOSDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  *length = sizeof(USBD_Dev_BOSDesc);
  return (uint8_t*)USBD_Dev_BOSDesc;
}
#endif /* (USBD_LPM_ENABLED == 1) */

/**
  * @brief  Create the serial number string descriptor
  * @param  None
  * @retval None
  */
static void Get_SerialNum(void)
{
  uint32_t deviceserial0, deviceserial1, deviceserial2;

  deviceserial0 = *(uint32_t *) DEVICE_ID1;
  deviceserial1 = *(uint32_t *) DEVICE_ID2;
  deviceserial2 = *(uint32_t *) DEVICE_ID3;

  deviceserial0 += deviceserial2;

  if (deviceserial0 != 0)
  {
    IntToUnicode(deviceserial0, &USBD_StringSerial[2], 8);
    IntToUnicode(deviceserial1, &USBD_StringSerial[18], 4);
  }
}

/**
  * @brief  Convert Hex 32Bits value into char
  * @param  value: value to convert
  * @param  pbuf: pointer to the buffer
  * @param  len: buffer length
  * @retval None
  */
static void IntToUnicode(uint32_t value, uint8_t * pbuf, uint8_t len)
{
  uint8_t idx = 0;

  for (idx = 0; idx < len; idx++)
  {
    if (((value >> 28)) < 0xA)
    {
      pbuf[2 * idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2 * idx] = (value >> 28) + 'A' - 10;
    }

    value = value << 4;

    pbuf[2 * idx + 1] = 0;
  }
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
