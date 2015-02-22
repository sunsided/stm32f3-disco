/**
  ******************************************************************************
  * @file    Demonstrations/Inc/usbd_desc.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-June-2014
  * @brief   Header for usbd_desc.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_DESC_H
#define __USBD_DESC_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define         DEVICE_ID1          (0x1FFFF7AC)
#define         DEVICE_ID2          (0x1FFFF7B0)
#define         DEVICE_ID3          (0x1FFFF7B4)

#define  USB_SIZ_STRING_SERIAL               0x1A
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern USBD_DescriptorsTypeDef HID_Desc;

#endif /* __USBD_DESC_H */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
