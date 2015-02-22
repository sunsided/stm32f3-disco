#include "main.h"

extern "C"
{
	extern PCD_HandleTypeDef hpcd;
	USBD_HandleTypeDef hUSBDDevice;

	/* UART handler declared in "usbd_cdc_interface.c" file */
	extern UART_HandleTypeDef UartHandle;

	/* TIM handler declared in "usbd_cdc_interface.c" file */
	extern TIM_HandleTypeDef TimHandle;
}


/**
 * @brief Initializes the LEDs.
 *
 * This function initializes the LEDs used in this example.
 */
void InitializeLEDs()
{
    __GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef gpioStructure;
    gpioStructure.Pin = GPIO_PIN_8;
    gpioStructure.Mode = GPIO_MODE_OUTPUT_PP;
    gpioStructure.Pull = GPIO_PULLUP;
    gpioStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOE, &gpioStructure);

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
}

TIM_HandleTypeDef TIM_Handle;

/**
 * @brief Initializes the timer.
 *
 * This function initializes the TIM2 timer.
 */
void InitializeTimer()
{
    __TIM2_CLK_ENABLE();

    TIM_Handle.Instance = TIM2;
	TIM_Handle.Init.Prescaler = 40000;
	TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM_Handle.Init.Period = 500;
	TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_Handle.Init.RepetitionCounter = 0;

    TIM_ClockConfigTypeDef clockSourceConfig;
    clockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    clockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
    clockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
    HAL_TIM_ConfigClockSource(&TIM_Handle, &clockSourceConfig); // TODO: check return value

	HAL_TIM_Base_Init(&TIM_Handle); // TODO: check return value

	// start the timer
	HAL_TIM_Base_Start_IT(&TIM_Handle); // TODO: check return value

    // allow timer interrupt
	__HAL_TIM_ENABLE_IT(&TIM_Handle, TIM_IT_UPDATE);
}

/**
 * @brief Enables the timer interrupt.
 *
 * This function enables the TIM2 timer interrupt.
 */
void EnableTimerInterrupt()
{
	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/**
 * @brief Configures the PA8 pin to follow the system clock (or system clock input).
 */
void InitializeMCOGPIO()
{
	__GPIOA_CLK_ENABLE();

	/* Configure MCO (PA8) */
	GPIO_InitTypeDef gpioStructure;
	gpioStructure.Pin = GPIO_PIN_8;
	gpioStructure.Speed = GPIO_SPEED_HIGH;
	gpioStructure.Mode = GPIO_MODE_AF_PP;
	gpioStructure.Pull = GPIO_NOPULL;
	gpioStructure.Alternate = GPIO_AF0_MCO;
	HAL_GPIO_Init(GPIOA, &gpioStructure);

	/* Output HSE clock on MCO pin (PA8) */
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCOSOURCE_HSE, RCC_MCO_NODIV);
}

/**
 * @brief Main entry point.
 * @param argc argument count when semihosting
 * @param argv argument vector when semihosting
 * @return arbitrary exit code, should be unused as this function should never return.
 */
int main(__attribute__((unused)) int argc, __attribute__((unused)) char* argv[])
{
    InitializeLEDs();
    InitializeTimer();
    EnableTimerInterrupt();
    InitializeMCOGPIO();

    /* Init Device Library */
	USBD_Init(&hUSBDDevice, &VCP_Desc, 0);

	/* Add Supported Class */
	USBD_RegisterClass(&hUSBDDevice, &USBD_CDC);

	/* Add CDC Interface Class */
	USBD_CDC_RegisterInterface(&hUSBDDevice, &USBD_CDC_fops);

	/* Start Device Process */
	USBD_Start(&hUSBDDevice);

    for (;;)
    {
    }

	// yup
	return 0;
}

/**
 * @brief System Tick Handler
 */
extern "C" void SysTick_Handler()
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

/**
 * @brief TIM2 Timer Interrupt Handler
 */
extern "C" void TIM2_IRQHandler()
{
    // check the interrupt status;
    // if the interrupt is SET, toggle the LED
    if (__HAL_TIM_GET_FLAG(&TIM_Handle, TIM_IT_UPDATE) != RESET)
    {
    	__HAL_TIM_CLEAR_FLAG(&TIM_Handle, TIM_IT_UPDATE);
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
    }
}


/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
extern "C" void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
extern "C" void HardFault_Handler(void)
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
extern "C" void MemManage_Handler(void)
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
extern "C" void BusFault_Handler(void)
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
extern "C" void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
extern "C" void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
extern "C" void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
extern "C" void PendSV_Handler(void)
{
}


/**
  * @brief  This function handles USB Handler.
  * @param  None
  * @retval None
  */
#if defined (USE_USB_INTERRUPT_DEFAULT)
extern "C" void USB_LP_CAN1_RX0_IRQHandler(void)
#elif defined (USE_USB_INTERRUPT_REMAPPED)
extern "C" void USB_LP_IRQHandler(void)
#endif
{
	HAL_PCD_IRQHandler(&hpcd);
}

/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  */
extern "C" void USARTx_DMA_TX_IRQHandler(void)
{
	HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

/**
  * @brief  This function handles UART interrupt request.
  * @param  None
  * @retval None
  */
extern "C" void USARTx_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle);
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
extern "C" void TIMx_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TimHandle);
}
