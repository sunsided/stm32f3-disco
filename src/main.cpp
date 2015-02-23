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
	if (HAL_TIM_ConfigClockSource(&TIM_Handle, &clockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_TIM_Base_Init(&TIM_Handle) != HAL_OK)
	{
		Error_Handler();
	}

	// start the timer
	if (HAL_TIM_Base_Start_IT(&TIM_Handle) != HAL_OK)
	{
		Error_Handler();
	}

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

	/* moving in Manhattan circles */
	const uint8_t stepWidth = 3;
	const uint8_t segmentLength = 40;
	uint8_t position = 0;
	int8_t x_increment = 0, y_increment = 0;

	for (;;)
	{
		HAL_Delay(25);

		/* advance the position state */
		++position;
		if (position >= 4 * segmentLength)
		{
			position = 0;
		}

		/* translate position to pointer movement */
		if (position >= 3 * segmentLength)
		{
			x_increment = +stepWidth;
			y_increment = -stepWidth;
		}
		else if (position >= 2 * segmentLength)
		{
			x_increment = -stepWidth;
			y_increment = -stepWidth;
		}
		else if (position >= 1 * segmentLength)
		{
			x_increment = -stepWidth;
			y_increment = +stepWidth;
		}
		else // if (position >= 0*segmentLength)
		{
			x_increment = +stepWidth;
			y_increment = +stepWidth;
		}

		/* now move the pointer */
		uint8_t HID_Buffer[4];

		// 5 padding bits (7:3) and 3 button bits (2:0, 0 being left button)
		HID_Buffer[0] = 0;
		// X axis
		HID_Buffer[1] = x_increment;
		// Y axis, screen origin is top left, so positive means down
		HID_Buffer[2] = y_increment;
		// wheel
		HID_Buffer[3] = 0;

		USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
	}

	// yup
	return 0;
}
