/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usb_device.c
  * @version        : v1.0_Cube
  * @brief          : This file implements the USB Device
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

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

/* USER CODE BEGIN Includes */
#include "usbd_composite.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Device Core handle declaration. */
#if defined (USE_OTG_HS)
USBD_HandleTypeDef hUsbDeviceHS;
#define pUsbDevice &hUsbDeviceHS
#endif

#if defined (USE_OTG_FS)
USBD_HandleTypeDef hUsbDeviceFS;
#define pUsbDevice &hUsbDeviceFS
#endif

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_DEVICE_Init(void)
{
  /* USER CODE BEGIN USB_DEVICE_Init_PreTreatment */

  /* USER CODE END USB_DEVICE_Init_PreTreatment */

  /* Init Device Library, add supported class and start the library. */
#if defined (USE_OTG_HS)
  if (USBD_Init(pUsbDevice, &Dev_Desc, DEVICE_HS) != USBD_OK)
#endif
#if defined (USE_OTG_FS)
  if (USBD_Init(pUsbDevice, &Dev_Desc, DEVICE_FS) != USBD_OK)
#endif
  {
    Error_Handler();
  }
  if (USBD_RegisterClass(pUsbDevice, &USBD_Composite) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_CDC_RegisterInterface(pUsbDevice, &USBD_Interface_fops) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_Start(pUsbDevice) != USBD_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN USB_DEVICE_Init_PostTreatment */

  /* USER CODE END USB_DEVICE_Init_PostTreatment */
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
