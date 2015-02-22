#include "main.h"

USBD_HandleTypeDef USBD_Device;
TIM_HandleTypeDef TIM_Handle;

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

    /* Init USB Device Library */
    USBD_Init(&USBD_Device, &HID_Desc, 0);

    /* Register the USB HID class */
    USBD_RegisterClass(&USBD_Device, &USBD_HID);

    /* Start Device Process */
    USBD_Start(&USBD_Device);

    for (;;)
    {
    }

	// yup
	return 0;
}
