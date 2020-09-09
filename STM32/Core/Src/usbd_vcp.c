/*-- File description -------------------------------------------------------*/
/**
 *   @file:    usbd_vcp.c
 *
 *   @author:  valeriy.williams.
 *   @company: Lab
 */

#include "usbd_vcp.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
#include "RoundBuffer.h"
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
#if (NUM_OF_CDC_UARTS > 1)
uint8_t _usb_cdc1_TxBuff[512] = { 0 };
RoundBuffer_t USB_CDC1_TxBuffer[] = {_usb_cdc1_TxBuff, sizeof(_usb_cdc1_TxBuff), 0, 0};
uint8_t _usb_cdc1_RxBuff[512] = { 0 };
RoundBuffer_t USB_CDC1_RxBuffer[] = {_usb_cdc1_RxBuff, sizeof(_usb_cdc1_RxBuff), 0, 0};
#endif

#if (NUM_OF_CDC_UARTS > 1)
uint8_t _usb_cdc2_TxBuff[512] = { 0 };
RoundBuffer_t USB_CDC2_TxBuffer[] = {_usb_cdc2_TxBuff, sizeof(_usb_cdc2_TxBuff), 0, 0};
uint8_t _usb_cdc2_RxBuff[512] = { 0 };
RoundBuffer_t USB_CDC2_RxBuffer[] = {_usb_cdc2_RxBuff, sizeof(_usb_cdc2_RxBuff), 0, 0};
#endif

#if (NUM_OF_CDC_UARTS > 2)
uint8_t _usb_cdc3_TxBuff[512] = { 0 };
RoundBuffer_t USB_CDC3_TxBuffer[] = {_usb_cdc3_TxBuff, sizeof(_usb_cdc3_TxBuff), 0, 0};
uint8_t _usb_cdc3_RxBuff[512] = { 0 };
RoundBuffer_t USB_CDC3_RxBuffer[] = {_usb_cdc3_RxBuff, sizeof(_usb_cdc3_RxBuff), 0, 0};
#endif

#if (NUM_OF_CDC_UARTS > 3)
uint8_t _usb_cdc4_TxBuff[512] = { 0 };
RoundBuffer_t USB_CDC4_TxBuffer[] = {_usb_cdc4_TxBuff, sizeof(_usb_cdc4_TxBuff), 0, 0};
uint8_t _usb_cdc4_RxBuff[512] = { 0 };
RoundBuffer_t USB_CDC4_RxBuffer[] = {_usb_cdc4_RxBuff, sizeof(_usb_cdc4_RxBuff), 0, 0};
#endif

/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
int8_t USB_VCP_ConfigCallback(uint8_t cmd, uint8_t* pbuf, uint16_t length, uint8_t interfaceNumber)
{
	static uint8_t tempSettBuff[7] = { 0 };
	
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
	{
		memcpy(tempSettBuff, pbuf, 7);
	}
    break;

    case CDC_GET_LINE_CODING:
	{
		memcpy(pbuf, tempSettBuff, 7);
	}
    break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void USB_VCP_SendData(uint8_t *buffer, uint16_t length, uint8_t interfaceNumber)
{
	switch (interfaceNumber)
	{
#if (NUM_OF_CDC_UARTS > 0)
		case CDC_ITF_NUMBER_1:
		{
			RoundBuffer_AddArray(USB_CDC1_TxBuffer, buffer, length);
		}
		break;
#endif
		
#if (NUM_OF_CDC_UARTS > 1)
		case CDC_ITF_NUMBER_2:
		{
			RoundBuffer_AddArray(USB_CDC2_TxBuffer, buffer, length);
		}
		break;
#endif

#if (NUM_OF_CDC_UARTS > 2)
		case CDC_ITF_NUMBER_3:
		{
			RoundBuffer_AddArray(USB_CDC3_TxBuffer, buffer, length);
		}
		break;
#endif

#if (NUM_OF_CDC_UARTS > 3)
		case CDC_ITF_NUMBER_4:
		{
			RoundBuffer_AddArray(USB_CDC4_TxBuffer, buffer, length);
		}
		break;
#endif
		
		default:
		{
		}
		break;
	}
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
#if (NUM_OF_CDC_UARTS > 0)
void USB_VCP_IF1_SendData(uint8_t *buffer, uint16_t length)
{
	USB_VCP_SendData(buffer, length, CDC_ITF_NUMBER_1);
}
#endif

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
#if (NUM_OF_CDC_UARTS > 1)
void USB_VCP_IF2_SendData(uint8_t *buffer, uint16_t length)
{
	USB_VCP_SendData(buffer, length, CDC_ITF_NUMBER_2);
}
#endif

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
#if (NUM_OF_CDC_UARTS > 2)
void USB_VCP_IF3_SendData(uint8_t *buffer, uint16_t length)
{
	USB_VCP_SendData(buffer, length, CDC_ITF_NUMBER_3);
}
#endif
/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
#if (NUM_OF_CDC_UARTS > 3)
void USB_VCP_IF4_SendData(uint8_t *buffer, uint16_t length)
{
	USB_VCP_SendData(buffer, length, CDC_ITF_NUMBER_4);
}
#endif

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void USB_VCP_DataReceivedCallback(uint8_t* buffer, uint16_t length, uint8_t interfaceNumber)
{
	switch (interfaceNumber)
	{
#if (NUM_OF_CDC_UARTS > 0)
		case CDC_ITF_NUMBER_1:
		{
			RoundBuffer_AddArray(USB_CDC1_RxBuffer, buffer, length);
		}
		break;
#endif
		
#if (NUM_OF_CDC_UARTS > 1)
		case CDC_ITF_NUMBER_2:
		{
			RoundBuffer_AddArray(USB_CDC2_RxBuffer, buffer, length);
		}
		break;
#endif

#if (NUM_OF_CDC_UARTS > 2)
		case CDC_ITF_NUMBER_3:
		{
			RoundBuffer_AddArray(USB_CDC3_RxBuffer, buffer, length);
		}
		break;
#endif

#if (NUM_OF_CDC_UARTS > 3)
		case CDC_ITF_NUMBER_4:
		{
			RoundBuffer_AddArray(USB_CDC4_RxBuffer, buffer, length);
		}
		break;
#endif
		
		default:
		{
		}
		break;
	}
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void USB_VCP_CableConnected(PCD_HandleTypeDef *hpcd)
{
	//STATUS_HW_SET(STATUS_HW_USB_CONNECTED);
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void USB_VCP_CableDisconnected(PCD_HandleTypeDef *hpcd)
{
	//STATUS_HW_CLEAR(STATUS_HW_USB_CONNECTED);
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void USB_VCP_Run(void)
{	
	uint16_t rxDataLength = 0;
	uint16_t txDataLength = 0;
#if defined (USE_OTG_FS)
  static uint8_t dataTxBuffer[CDC_DATA_FS_MAX_PACKET_SIZE];
#endif
#if defined (USE_OTG_HS)
  static uint8_t dataTxBuffer[CDC_DATA_HS_MAX_PACKET_SIZE];
#endif
	

#if (NUM_OF_CDC_UARTS > 0)
	//Parsing of received data
	rxDataLength = RoundBuffer_GetLoad(USB_CDC1_RxBuffer);
	if(rxDataLength > 0)
	{	
		for(uint16_t i = 0; i < rxDataLength; i++)
		{
			RoundBuffer_AddByte(USB_CDC1_TxBuffer, RoundBuffer_GetByte(USB_CDC1_RxBuffer));
		}
	}
	
	//Sending prepeared data
	txDataLength = RoundBuffer_GetLoad(USB_CDC1_TxBuffer);
	if(txDataLength > 0)
	{
    if(CDC_CheckTransmitAvailable(CDC_ITF_NUMBER_1))
    {
      if(txDataLength > sizeof(dataTxBuffer))
      {
        txDataLength = sizeof(dataTxBuffer);
      }
      
      txDataLength = RoundBuffer_GetArray(USB_CDC1_TxBuffer, dataTxBuffer, txDataLength);
      
      CDC_Transmit(dataTxBuffer, txDataLength, CDC_ITF_NUMBER_1);
    }
	}
#endif
	
#if (NUM_OF_CDC_UARTS > 1)
	//Parsing of received data
	rxDataLength = RoundBuffer_GetLoad(USB_CDC2_RxBuffer);
	if(rxDataLength > 0)
	{	
		for(uint16_t i = 0; i < rxDataLength; i++)
		{
			RoundBuffer_AddByte(USB_CDC2_TxBuffer, RoundBuffer_GetByte(USB_CDC2_RxBuffer));
		}
	}
	
	//Sending prepeared data
	txDataLength = RoundBuffer_GetLoad(USB_CDC2_TxBuffer);
	if(txDataLength > 0)
	{
    if(CDC_CheckTransmitAvailable(CDC_ITF_NUMBER_2))
    {
      if(txDataLength > sizeof(dataTxBuffer))
      {
        txDataLength = sizeof(dataTxBuffer);
      }
      
      txDataLength = RoundBuffer_GetArray(USB_CDC2_TxBuffer, dataTxBuffer, txDataLength);
      
      CDC_Transmit(dataTxBuffer, txDataLength, CDC_ITF_NUMBER_2);
    }
	}
#endif
	
#if (NUM_OF_CDC_UARTS > 2)
	//Parsing of received data
	rxDataLength = RoundBuffer_GetLoad(USB_CDC3_RxBuffer);
	if(rxDataLength > 0)
	{	
		for(uint16_t i = 0; i < rxDataLength; i++)
		{
			RoundBuffer_AddByte(USB_CDC3_TxBuffer, RoundBuffer_GetByte(USB_CDC3_RxBuffer));
		}
	}
	
	//Sending prepeared data
	txDataLength = RoundBuffer_GetLoad(USB_CDC3_TxBuffer);
	if(txDataLength > 0)
	{
    if(CDC_CheckTransmitAvailable(CDC_ITF_NUMBER_3))
    {
      if(txDataLength > sizeof(dataTxBuffer))
      {
        txDataLength = sizeof(dataTxBuffer);
      }
      
      txDataLength = RoundBuffer_GetArray(USB_CDC3_TxBuffer, dataTxBuffer, txDataLength);
      
      CDC_Transmit(dataTxBuffer, txDataLength, CDC_ITF_NUMBER_3);
    }
	}
#endif
	
#if (NUM_OF_CDC_UARTS > 3)
	//Parsing of received data
	rxDataLength = RoundBuffer_GetLoad(USB_CDC4_RxBuffer);
	if(rxDataLength > 0)
	{	
		for(uint16_t i = 0; i < rxDataLength; i++)
		{
			RoundBuffer_AddByte(USB_CDC4_TxBuffer, RoundBuffer_GetByte(USB_CDC4_RxBuffer));
		}
	}
	
	//Sending prepeared data
	txDataLength = RoundBuffer_GetLoad(USB_CDC4_TxBuffer);
	if(txDataLength > 0)
	{
    if(CDC_CheckTransmitAvailable(CDC_ITF_NUMBER_4))
    {
      if(txDataLength > sizeof(dataTxBuffer))
      {
        txDataLength = sizeof(dataTxBuffer);
      }
      
      txDataLength = RoundBuffer_GetArray(USB_CDC4_TxBuffer, dataTxBuffer, txDataLength);
      
      CDC_Transmit(dataTxBuffer, txDataLength, CDC_ITF_NUMBER_4);
    }
	}
#endif
}

/*-- EOF --------------------------------------------------------------------*/
