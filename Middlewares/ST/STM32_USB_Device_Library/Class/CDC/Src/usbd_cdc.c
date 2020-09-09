/**
  ******************************************************************************
  * @file    usbd_cdc.c
  * @author  MCD Application Team
  * @brief   This file provides the high layer firmware functions to manage the
  *          following functionalities of the USB CDC Class:
  *           - Initialization and Configuration of high and low layer
  *           - Enumeration as CDC Device (and enumeration for each implemented memory interface)
  *           - OUT/IN data transfer
  *           - Command IN transfer (class requests management)
  *           - Error management
  *
  *  @verbatim
  *
  *          ===================================================================
  *                                CDC Class Driver Description
  *          ===================================================================
  *           This driver manages the "Universal Serial Bus Class Definitions for Communications Devices
  *           Revision 1.2 November 16, 2007" and the sub-protocol specification of "Universal Serial Bus
  *           Communications Class Subclass Specification for PSTN Devices Revision 1.2 February 9, 2007"
  *           This driver implements the following aspects of the specification:
  *             - Device descriptor management
  *             - Configuration descriptor management
  *             - Enumeration as CDC device with 2 data endpoints (IN and OUT) and 1 command endpoint (IN)
  *             - Requests management (as described in section 6.2 in specification)
  *             - Abstract Control Model compliant
  *             - Union Functional collection (using 1 IN endpoint for control)
  *             - Data interface class
  *
  *           These aspects may be enriched or modified for a specific user application.
  *
  *            This driver doesn't implement the following aspects of the specification
  *            (but it is possible to manage these features with some modifications on this driver):
  *             - Any class-specific aspect relative to communication classes should be managed by user application.
  *             - All communication classes other than PSTN are not managed
  *
  *  @endverbatim
  *
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

/* BSPDependencies
- "stm32xxxxx_{eval}{discovery}{nucleo_144}.c"
- "stm32xxxxx_{eval}{discovery}_io.c"
EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc.h"
#include "usbd_ctlreq.h"

#include "usbhelper.h"
#include "cdchelper.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_CDC
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_CDC_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_CDC_Private_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_CDC_Private_Macros
  * @{
  */

/**
  * @}
  */


/** @defgroup USBD_CDC_Private_FunctionPrototypes
  * @{
  */

static uint8_t USBD_CDC_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_CDC_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_CDC_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t USBD_CDC_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_CDC_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_CDC_EP0_RxReady(USBD_HandleTypeDef *pdev);

static uint8_t *USBD_CDC_GetFSCfgDesc(uint16_t *length);
static uint8_t *USBD_CDC_GetHSCfgDesc(uint16_t *length);
static uint8_t *USBD_CDC_GetOtherSpeedCfgDesc(uint16_t *length);
static uint8_t *USBD_CDC_GetOtherSpeedCfgDesc(uint16_t *length);
uint8_t *USBD_CDC_GetDeviceQualifierDescriptor(uint16_t *length);

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_CDC_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

/**
  * @}
  */

/** @defgroup USBD_CDC_Private_Variables
  * @{
  */


/* CDC interface class callbacks structure */
USBD_ClassTypeDef  USBD_CDC =
{
  USBD_CDC_Init,
  USBD_CDC_DeInit,
  USBD_CDC_Setup,
  NULL,                 /* EP0_TxSent, */
  USBD_CDC_EP0_RxReady,
  USBD_CDC_DataIn,
  USBD_CDC_DataOut,
  NULL,
  NULL,
  NULL,
  USBD_CDC_GetHSCfgDesc,
  USBD_CDC_GetFSCfgDesc,
  USBD_CDC_GetOtherSpeedCfgDesc,
  USBD_CDC_GetDeviceQualifierDescriptor,
};

/* bespoke struct for this device; struct members are added and removed as needed */
struct configuration_1
{
  struct configuration_descriptor config;
  struct cdc_interface cdc[NUM_OF_CDC_UARTS];
};

/* USB CDC device HS Configuration Descriptor */
__ALIGN_BEGIN static const struct configuration_1 USBD_Composite_CfgHSDesc __ALIGN_END =
{
  {
    /* Configuration Descriptor */
    sizeof(struct configuration_descriptor),         /* bLength */
    USB_DESC_TYPE_CONFIGURATION,                     /* bDescriptorType */
    USB_UINT16(sizeof(USBD_Composite_CfgHSDesc)),    /* wTotalLength */
    USBD_MAX_NUM_INTERFACES,                         /* bNumInterfaces */
    0x01,                                            /* bConfigurationValue */
    0x00,                                            /* iConfiguration */
    0x80,                                            /* bmAttributes */
    0x32,                                            /* MaxPower 0 mA */
  },

  {
#if (NUM_OF_CDC_UARTS > 0)
    /* CDC1 */
    CDC_HS_DESCRIPTOR(CDC_ITF_CMD_1, CDC_ITF_DATA_1, CDC_EP_CMD_1, CDC_EP_DATA_OUT_1, CDC_EP_DATA_IN_1)
#endif
#if (NUM_OF_CDC_UARTS > 1)
    /* CDC2 */
    CDC_HS_DESCRIPTOR(CDC_ITF_CMD_2, CDC_ITF_DATA_2, CDC_EP_CMD_2, CDC_EP_DATA_OUT_2, CDC_EP_DATA_IN_2)
#endif
#if (NUM_OF_CDC_UARTS > 2)
    /* CDC3 */
    CDC_HS_DESCRIPTOR(CDC_ITF_CMD_3, CDC_ITF_DATA_3, CDC_EP_CMD_3, CDC_EP_DATA_OUT_3, CDC_EP_DATA_IN_3)
#endif
#if (NUM_OF_CDC_UARTS > 3)
    /* CDC4 */
    CDC_HS_DESCRIPTOR(CDC_ITF_CMD_4, CDC_ITF_DATA_4, CDC_EP_CMD_4, CDC_EP_DATA_OUT_4, CDC_EP_DATA_IN_4)
#endif
  },
};


/* USB CDC device FS Configuration Descriptor */
__ALIGN_BEGIN static const struct configuration_1 USBD_Composite_CfgFSDesc __ALIGN_END =
{
  {
    /* Configuration Descriptor */
    sizeof(struct configuration_descriptor),         /* bLength */
    USB_DESC_TYPE_CONFIGURATION,                     /* bDescriptorType */
    USB_UINT16(sizeof(USBD_Composite_CfgFSDesc)),    /* wTotalLength */
    USBD_MAX_NUM_INTERFACES,                         /* bNumInterfaces */
    0x01,                                            /* bConfigurationValue */
    0x00,                                            /* iConfiguration */
    0x80,                                            /* bmAttributes */
    0x32,                                            /* MaxPower 0 mA */
  },

  {
#if (NUM_OF_CDC_UARTS > 0)
    /* CDC1 */
    CDC_HS_DESCRIPTOR(CDC_ITF_CMD_1, CDC_ITF_DATA_1, CDC_EP_CMD_1, CDC_EP_DATA_OUT_1, CDC_EP_DATA_IN_1)
#endif
#if (NUM_OF_CDC_UARTS > 1)
    /* CDC2 */
    CDC_HS_DESCRIPTOR(CDC_ITF_CMD_2, CDC_ITF_DATA_2, CDC_EP_CMD_2, CDC_EP_DATA_OUT_2, CDC_EP_DATA_IN_2)
#endif
#if (NUM_OF_CDC_UARTS > 2)
    /* CDC3 */
    CDC_HS_DESCRIPTOR(CDC_ITF_CMD_3, CDC_ITF_DATA_3, CDC_EP_CMD_3, CDC_EP_DATA_OUT_3, CDC_EP_DATA_IN_3)
#endif
#if (NUM_OF_CDC_UARTS > 3)
    /* CDC4 */
    CDC_HS_DESCRIPTOR(CDC_ITF_CMD_4, CDC_ITF_DATA_4, CDC_EP_CMD_4, CDC_EP_DATA_OUT_4, CDC_EP_DATA_IN_4)
#endif
  },
};

/* USB CDC device FS Configuration Descriptor */
__ALIGN_BEGIN static const struct configuration_1 USBD_Composite_CfgOtherDesc __ALIGN_END =
{
  {
    /* Configuration Descriptor */
    sizeof(struct configuration_descriptor),         /* bLength */
    USB_DESC_TYPE_CONFIGURATION,                     /* bDescriptorType */
    USB_UINT16(sizeof(USBD_Composite_CfgOtherDesc)), /* wTotalLength */
    USBD_MAX_NUM_INTERFACES,                         /* bNumInterfaces */
    0x01,                                            /* bConfigurationValue */
    0x00,                                            /* iConfiguration */
    0x80,                                            /* bmAttributes */
    0x32,                                            /* MaxPower 0 mA */
  },

  {
#if (NUM_OF_CDC_UARTS > 0)
    /* CDC1 */
    CDC_OTHER_SPEED_DESCRIPTOR(CDC_ITF_CMD_1, CDC_ITF_DATA_1, CDC_EP_CMD_1, CDC_EP_DATA_OUT_1, CDC_EP_DATA_IN_1)
#endif
#if (NUM_OF_CDC_UARTS > 1)
    /* CDC2 */
    CDC_OTHER_SPEED_DESCRIPTOR(CDC_ITF_CMD_2, CDC_ITF_DATA_2, CDC_EP_CMD_2, CDC_EP_DATA_OUT_2, CDC_EP_DATA_IN_2)
#endif
#if (NUM_OF_CDC_UARTS > 2)
    /* CDC3 */
    CDC_OTHER_SPEED_DESCRIPTOR(CDC_ITF_CMD_3, CDC_ITF_DATA_3, CDC_EP_CMD_3, CDC_EP_DATA_OUT_3, CDC_EP_DATA_IN_3)
#endif
#if (NUM_OF_CDC_UARTS > 3)
    /* CDC4 */
    CDC_OTHER_SPEED_DESCRIPTOR(CDC_ITF_CMD_4, CDC_ITF_DATA_4, CDC_EP_CMD_4, CDC_EP_DATA_OUT_4, CDC_EP_DATA_IN_4)
#endif
  },
};

/* endpoint numbers and "instance" (base register address) for each UART */
static const USBD_CDC_ParamsTypeDef USBD_CfgParams[NUM_OF_CDC_UARTS] = 
{
#if (NUM_OF_CDC_UARTS > 0)
  {
	.interfaceNumber = CDC_ITF_NUMBER_1,
    .data_in_ep  = CDC_EP_DATA_IN_1,
    .data_out_ep = CDC_EP_DATA_OUT_1,
    .command_ep  = CDC_EP_CMD_1,
    .command_itf = CDC_ITF_CMD_1,
  },
#endif
#if (NUM_OF_CDC_UARTS > 1)
  {
	.interfaceNumber = CDC_ITF_NUMBER_2,
    .data_in_ep  = CDC_EP_DATA_IN_2,
    .data_out_ep = CDC_EP_DATA_OUT_2,
    .command_ep  = CDC_EP_CMD_2,
    .command_itf = CDC_ITF_CMD_2,
  },
#endif
#if (NUM_OF_CDC_UARTS > 2)
  {
	.interfaceNumber = CDC_ITF_NUMBER_3,
    .data_in_ep  = CDC_EP_DATA_IN_3,
    .data_out_ep = CDC_EP_DATA_OUT_3,
    .command_ep  = CDC_EP_CMD_3,
    .command_itf = CDC_ITF_CMD_3,
  },
#endif
#if (NUM_OF_CDC_UARTS > 3)
  {
	.interfaceNumber = CDC_ITF_NUMBER_4,
    .data_in_ep  = CDC_EP_DATA_IN_4,
    .data_out_ep = CDC_EP_DATA_OUT_4,
    .command_ep  = CDC_EP_CMD_4,
    .command_itf = CDC_ITF_CMD_4,
  },
#endif
};

/* context for each and every UART managed by this CDC implementation */
static USBD_CDC_HandleTypeDef cdcContextData[NUM_OF_CDC_UARTS];

/**
  * @}
  */

/** @defgroup USBD_CDC_Private_Functions
  * @{
  */

/**
  * @brief  USBD_CDC_Init
  *         Initialize the CDC interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_CDC_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);
  USBD_CDC_HandleTypeDef *hcdc = cdcContextData;
  uint8_t index;

  if (hcdc == NULL)
  {
    pdev->pClassData = NULL;
    return (uint8_t)USBD_EMEM;
  }

  pdev->pClassData = (void *)hcdc;

  
  for (index = 0; index < NUM_OF_CDC_UARTS; index++, hcdc++)
  {
    if (pdev->dev_speed == USBD_SPEED_HIGH)
    {
      /* Open EP IN */
      USBD_LL_OpenEP(pdev, USBD_CfgParams[index].data_in_ep, USBD_EP_TYPE_BULK, CDC_DATA_HS_IN_PACKET_SIZE);
      pdev->ep_in[USBD_CfgParams[index].data_in_ep & 0xFU].is_used = 1U;


      /* Open EP OUT */
      USBD_LL_OpenEP(pdev, USBD_CfgParams[index].data_out_ep, USBD_EP_TYPE_BULK, CDC_DATA_HS_OUT_PACKET_SIZE);
      pdev->ep_out[USBD_CfgParams[index].data_out_ep & 0xFU].is_used = 1U;


      /* Set bInterval for CMD Endpoint */
      pdev->ep_in[USBD_CfgParams[index].command_ep & 0xFU].bInterval = CDC_HS_BINTERVAL;

      /* Open Command IN EP */
      USBD_LL_OpenEP(pdev, USBD_CfgParams[index].command_ep, USBD_EP_TYPE_INTR, CDC_CMD_PACKET_SIZE);
      pdev->ep_in[USBD_CfgParams[index].command_ep & 0xFU].is_used = 1U;
    }
    else
    {
      /* Open EP IN */
      USBD_LL_OpenEP(pdev, USBD_CfgParams[index].data_in_ep, USBD_EP_TYPE_BULK, CDC_DATA_FS_IN_PACKET_SIZE);
      pdev->ep_in[USBD_CfgParams[index].data_in_ep & 0xFU].is_used = 1U;


      /* Open EP OUT */
      USBD_LL_OpenEP(pdev, USBD_CfgParams[index].data_out_ep, USBD_EP_TYPE_BULK, CDC_DATA_FS_OUT_PACKET_SIZE);
      pdev->ep_out[USBD_CfgParams[index].data_out_ep & 0xFU].is_used = 1U;


      /* Set bInterval for CMD Endpoint */
      pdev->ep_in[USBD_CfgParams[index].command_ep & 0xFU].bInterval = CDC_FS_BINTERVAL;

      /* Open Command IN EP */
      USBD_LL_OpenEP(pdev, USBD_CfgParams[index].command_ep, USBD_EP_TYPE_INTR, CDC_CMD_PACKET_SIZE);
      pdev->ep_in[USBD_CfgParams[index].command_ep & 0xFU].is_used = 1U;
    }

    /* Init Xfer states */
    hcdc->TxState = 0U;
    hcdc->RxState = 0U;

    /* Configure the UART peripheral */
    //Not used
  }


  /* Init  physical Interface components */
  ((USBD_CDC_ItfTypeDef *)pdev->pUserData)->Init();

//  /* Back to start hcdc pointer */
//  hcdc = cdcContextData;

//  /* Prepare Out endpoint to receive next packet */
//  for (index = 0; index < NUM_OF_CDC_UARTS; index++, hcdc++)
//  {
//    if (pdev->dev_speed == USBD_SPEED_HIGH)
//    {
//      USBD_LL_PrepareReceive(pdev, USBD_CfgParams[index].data_out_ep, hcdc->RxBuffer, CDC_DATA_HS_OUT_PACKET_SIZE);
//    }
//    else
//    {
//      USBD_LL_PrepareReceive(pdev, USBD_CfgParams[index].data_out_ep, hcdc->RxBuffer, CDC_DATA_FS_OUT_PACKET_SIZE);
//    }
//  }

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_CDC_Init
  *         DeInitialize the CDC layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_CDC_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);
  uint8_t ret = 0U;
  uint8_t index;

  for (index = 0; index < NUM_OF_CDC_UARTS; index++)
  {
    /* Close EP IN */
    USBD_LL_CloseEP(pdev, USBD_CfgParams[index].data_in_ep);
    pdev->ep_in[USBD_CfgParams[index].data_in_ep & 0xFU].is_used = 0U;

    /* Close EP OUT */
    USBD_LL_CloseEP(pdev, USBD_CfgParams[index].data_out_ep);
    pdev->ep_out[USBD_CfgParams[index].data_out_ep & 0xFU].is_used = 0U;

    /* Close Command IN EP */
    USBD_LL_CloseEP(pdev, USBD_CfgParams[index].command_ep);
    pdev->ep_in[USBD_CfgParams[index].command_ep & 0xFU].is_used = 0U;
    pdev->ep_in[USBD_CfgParams[index].command_ep & 0xFU].bInterval = 0U;

    /* DeInitialize the UART peripheral */
    //Not used
  }

  /* DeInit  physical Interface components */
  if (pdev->pClassData != NULL)
  {
    ((USBD_CDC_ItfTypeDef *)pdev->pUserData)->DeInit();
    pdev->pClassData = NULL;
  }

  return ret;
}

/**
  * @brief  USBD_CDC_Setup
  *         Handle the CDC specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t USBD_CDC_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)pdev->pClassData;
  uint8_t ifalt = 0U;
  uint16_t status_info = 0U;
  USBD_StatusTypeDef ret = USBD_OK;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
    case USB_REQ_TYPE_CLASS:
    {
      uint8_t index;
      
      for (index = 0; index < NUM_OF_CDC_UARTS; index++,hcdc++)
			{
        /* Check the interface number */
        if (USBD_CfgParams[index].command_itf != req->wIndex)
        {
          continue;
        }
        else
        {
          if (req->wLength != 0U)
          {
            if ((req->bmRequest & 0x80U) != 0U)
            {
              ((USBD_CDC_ItfTypeDef *)pdev->pUserData)->Control(req->bRequest, (uint8_t *)hcdc->data, req->wLength, USBD_CfgParams[index].interfaceNumber);
              
              (void)USBD_CtlSendData(pdev, (uint8_t *)hcdc->data, req->wLength);
            }
            else
            {
              hcdc->CmdOpCode = req->bRequest;
              hcdc->CmdLength = (uint8_t)req->wLength;

              (void)USBD_CtlPrepareRx(pdev, (uint8_t *)hcdc->data, req->wLength);
            }
          }
          else
          {
            ((USBD_CDC_ItfTypeDef *)pdev->pUserData)->Control(req->bRequest, (uint8_t *)req, 0U, USBD_CfgParams[index].interfaceNumber);
          }
        }
      }
    }
    break;

    case USB_REQ_TYPE_STANDARD:
    {
      switch (req->bRequest)
      {
        case USB_REQ_GET_STATUS:
        {
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
          {
            (void)USBD_CtlSendData(pdev, (uint8_t *)&status_info, 2U);
          }
          else
          {
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
          }
        }
        break;

        case USB_REQ_GET_INTERFACE:
        {
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
          {
            (void)USBD_CtlSendData(pdev, &ifalt, 1U);
          }
          else
          {
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
          }
        }
        break;

        case USB_REQ_SET_INTERFACE:
        {
          if (pdev->dev_state != USBD_STATE_CONFIGURED)
          {
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
          }
        }
        break;

        case USB_REQ_CLEAR_FEATURE:
        {
        }
        break;

      default:
        {
          USBD_CtlError(pdev, req);
          ret = USBD_FAIL;
        }
        break;
      }
    }
    break;

    default:
    {
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
    }
    break;
  }

  return (uint8_t)ret;
}

/**
  * @brief  USBD_CDC_DataIn
  *         Data sent on non-control IN endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t USBD_CDC_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  USBD_CDC_HandleTypeDef *hcdc;
  PCD_HandleTypeDef *hpcd = pdev->pData;
  uint8_t index;

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  hcdc = (USBD_CDC_HandleTypeDef *)pdev->pClassData;

  for (index = 0; index < NUM_OF_CDC_UARTS; index++,hcdc++)
  {
    if (USBD_CfgParams[index].data_in_ep == (epnum | 0x80))
    {
      if ((pdev->ep_in[epnum].total_length > 0U) && ((pdev->ep_in[epnum].total_length % hpcd->IN_ep[epnum].maxpacket) == 0U))
      {
        /* Update the packet total length */
        pdev->ep_in[epnum].total_length = 0U;

        /* Send ZLP */
        (void)USBD_LL_Transmit(pdev, epnum, NULL, 0U);
      }
      else
      {
        hcdc->TxState = 0U;
        ((USBD_CDC_ItfTypeDef *)pdev->pUserData)->TransmitCplt(hcdc->TxBuffer, &hcdc->TxLength, USBD_CfgParams[index].interfaceNumber);
      }
      
      break;
    }
  }
  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_CDC_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t USBD_CDC_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)pdev->pClassData;
  uint8_t index;

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  for (index = 0; index < NUM_OF_CDC_UARTS; index++,hcdc++)
  {
    if (USBD_CfgParams[index].data_out_ep == epnum)
    {
      /* Get the received data length */
      hcdc->RxLength = USBD_LL_GetRxDataSize(pdev, epnum);

      /* USB data will be immediately processed, this allow next USB traffic being
      NAKed till the end of the application Xfer */

      ((USBD_CDC_ItfTypeDef *)pdev->pUserData)->Receive(hcdc->RxBuffer, &hcdc->RxLength, USBD_CfgParams[index].interfaceNumber);

      break;
    }
  }

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_CDC_EP0_RxReady
  *         Handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_CDC_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)pdev->pClassData;
  uint8_t index;

  for (index = 0; index < NUM_OF_CDC_UARTS; index++,hcdc++)
  {
    if (USBD_CfgParams[index].command_itf != pdev->request.wIndex)
    {
      continue;
    }
    else
    {
      if ((pdev->pUserData != NULL) && (hcdc->CmdOpCode != 0xFFU))
      {
        ((USBD_CDC_ItfTypeDef *)pdev->pUserData)->Control(hcdc->CmdOpCode, (uint8_t *)hcdc->data, (uint16_t)hcdc->CmdLength, USBD_CfgParams[index].interfaceNumber);
        hcdc->CmdOpCode = 0xFFU;

      }
    }
  }

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_CDC_GetFSCfgDesc
  *         Return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_CDC_GetFSCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_Composite_CfgFSDesc);

  return (uint8_t *)&USBD_Composite_CfgFSDesc;
}

/**
  * @brief  USBD_CDC_GetHSCfgDesc
  *         Return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_CDC_GetHSCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_Composite_CfgHSDesc);

  return (uint8_t *)&USBD_Composite_CfgHSDesc;
}

/**
  * @brief  USBD_CDC_GetCfgDesc
  *         Return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_CDC_GetOtherSpeedCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_Composite_CfgOtherDesc);

  return (uint8_t *)&USBD_Composite_CfgOtherDesc;
}

/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t *USBD_CDC_GetDeviceQualifierDescriptor(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_CDC_DeviceQualifierDesc);

  return USBD_CDC_DeviceQualifierDesc;
}

uint8_t USBD_CDC_CheckSendingAvailable(USBD_HandleTypeDef *pdev, uint8_t interfaceNumber)
{
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)pdev->pClassData;
  uint8_t index;
  
  //Switch to correct handler
  for (index = 0; index < NUM_OF_CDC_UARTS; index++,hcdc++)
  {
    if (USBD_CfgParams[index].interfaceNumber == interfaceNumber)
    {
      break;
    }
  }

  if (hcdc->TxState != 0)
  {
    return (uint8_t)USBD_BUSY;
  }
  else
  {
    return (uint8_t)USBD_OK;
  }
}

/**
* @brief  USBD_CDC_RegisterInterface
  * @param  pdev: device instance
  * @param  fops: CD  Interface callback
  * @retval status
  */
uint8_t USBD_CDC_RegisterInterface(USBD_HandleTypeDef *pdev, USBD_CDC_ItfTypeDef *fops)
{
  if (fops == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  pdev->pUserData = fops;

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_CDC_SetTxBuffer
  * @param  pdev: device instance
  * @param  pbuff: Tx Buffer
  * @retval status
  */
uint8_t USBD_CDC_SetTxBuffer(USBD_HandleTypeDef *pdev, uint8_t interfaceNumber, uint8_t *pbuff, uint32_t length)
{
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)pdev->pClassData;
  uint8_t index;

  //Switch to correct handler
  for (index = 0; index < NUM_OF_CDC_UARTS; index++,hcdc++)
  {
    if (USBD_CfgParams[index].interfaceNumber == interfaceNumber)
    {
      break;
    }
  }
  
  hcdc->TxBuffer = pbuff;
  hcdc->TxLength = length;

  return (uint8_t)USBD_OK;
}


/**
  * @brief  USBD_CDC_SetRxBuffer
  * @param  pdev: device instance
  * @param  pbuff: Rx Buffer
  * @retval status
  */
uint8_t USBD_CDC_SetRxBuffer(USBD_HandleTypeDef *pdev, uint8_t interfaceNumber, uint8_t *pbuff)
{
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)pdev->pClassData;
  uint8_t index;

  //Switch to correct handler
  for (index = 0; index < NUM_OF_CDC_UARTS; index++,hcdc++)
  {
    if (USBD_CfgParams[index].interfaceNumber == interfaceNumber)
    {
      break;
    }
  }

  hcdc->RxBuffer = pbuff;

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_CDC_TransmitPacket
  *         Transmit packet on IN endpoint
  * @param  pdev: device instance
  * @retval status
  */
uint8_t USBD_CDC_TransmitPacket(USBD_HandleTypeDef *pdev, uint8_t interfaceNumber)
{
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)pdev->pClassData;
  USBD_StatusTypeDef ret = USBD_BUSY;
  uint8_t index;

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  //Switch to correct handler
  for (index = 0; index < NUM_OF_CDC_UARTS; index++,hcdc++)
  {
    if (USBD_CfgParams[index].interfaceNumber == interfaceNumber)
    {
      break;
    }
  }

  if (hcdc->TxState == 0U)
  {
    /* Tx Transfer in progress */
    hcdc->TxState = 1U;

    /* Update the packet total length */
    pdev->ep_in[USBD_CfgParams[index].data_in_ep & 0xFU].total_length = hcdc->TxLength;

    /* Transmit next packet */
    USBD_LL_Transmit(pdev, USBD_CfgParams[index].data_in_ep, hcdc->TxBuffer, hcdc->TxLength);

    ret = USBD_OK;
  }

  return (uint8_t)ret;
}


/**
  * @brief  USBD_CDC_ReceivePacket
  *         prepare OUT Endpoint for reception
  * @param  pdev: device instance
  * @retval status
  */
uint8_t USBD_CDC_ReceivePacket(USBD_HandleTypeDef *pdev, uint8_t interfaceNumber)
{
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)pdev->pClassData;
  uint8_t index;

  if (pdev->pClassData == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  //Switch to correct handler
  for (index = 0; index < NUM_OF_CDC_UARTS; index++,hcdc++)
  {
    if (USBD_CfgParams[index].interfaceNumber == interfaceNumber)
    {
      break;
    }
  }

  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    /* Prepare Out endpoint to receive next packet */
    USBD_LL_PrepareReceive(pdev, USBD_CfgParams[index].data_out_ep, hcdc->RxBuffer, CDC_DATA_HS_OUT_PACKET_SIZE);
  }
  else
  {
    /* Prepare Out endpoint to receive next packet */
    USBD_LL_PrepareReceive(pdev, USBD_CfgParams[index].data_out_ep, hcdc->RxBuffer, CDC_DATA_FS_OUT_PACKET_SIZE);
  }

  return (uint8_t)USBD_OK;
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
