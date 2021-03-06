/**
 ******************************************************************************
 * @file    Demonstrations/Src/stm32f3xx_it.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    18-June-2014
 * @brief   Main Interrupt Service Routines.
 *          This file provides all exceptions handler and
 *          peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_it.h"

/** @addtogroup STM32F3-Discovery_Demo STM32F3-Discovery_Demo
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd;
extern TIM_HandleTypeDef TIM_Handle;
/* Private function prototypes -----------------------------------------------*/

/******************************************************************************/
/*             Custom stuff                                                   */
/******************************************************************************/

/**
 * @brief TIM2 Timer Interrupt Handler
 */
void TIM2_IRQHandler()
{
	// check the interrupt status;
	// if the interrupt is SET, toggle the LED
	if (__HAL_TIM_GET_FLAG(&TIM_Handle, TIM_IT_UPDATE) != RESET)
	{
		__HAL_TIM_CLEAR_FLAG(&TIM_Handle, TIM_IT_UPDATE);
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
	}
}

/******************************************************************************/
/*             Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
	}
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles PendSV_Handler exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
	HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F3xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f3xx.s).                                               */
/******************************************************************************/

/**
 * @brief  This function handles USB Handler.
 * @param  None
 * @retval None
 */
#if defined (USE_USB_INTERRUPT_DEFAULT)
void USB_LP_CAN_RX0_IRQHandler(void)
#elif defined (USE_USB_INTERRUPT_REMAPPED)
void USB_LP_IRQHandler(void)
#endif
{
	HAL_PCD_IRQHandler(&hpcd);
}

/**
 * @brief  This function handles USB WakeUp interrupt request.
 * @param  None
 * @retval None
 */
#if defined (USE_USB_INTERRUPT_DEFAULT)
void USBWakeUp_IRQHandler(void)
#elif defined (USE_USB_INTERRUPT_REMAPPED)
void USBWakeUp_RMP_IRQHandler(void)
#endif
{
	__HAL_USB_EXTI_CLEAR_FLAG();
}

/**
 * @}
 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
