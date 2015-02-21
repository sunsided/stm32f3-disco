#include <stm32f3xx.h>
#include <stm32f3xx_hal.h>

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

    TIM_HandleTypeDef TIM_Handle;
    TIM_Handle.Instance = TIM2;
	TIM_Handle.Init.Prescaler = 40000;		// TODO: there's probably a prettier way
	TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM_Handle.Init.Period = 500;
	TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_Handle.Init.RepetitionCounter = 0;

    TIM_ClockConfigTypeDef clockSourceConfig;
    clockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&TIM_Handle, &clockSourceConfig);

	HAL_TIM_Base_Init(&TIM_Handle);

	// start the timer
	HAL_TIM_Base_Start_IT(&TIM_Handle);

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
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
}

/**
 * @brief Configures the PA8 pin to follow the system clock (or system clock input).
 */
void InitializeMCOGPIO() {
	__GPIOA_CLK_ENABLE();

	/* Configure MCO (PA8) */
	GPIO_InitTypeDef gpioStructure;
	gpioStructure.Pin = GPIO_PIN_8;
	gpioStructure.Speed = GPIO_SPEED_HIGH;
	gpioStructure.Mode = GPIO_MODE_AF_PP;
	gpioStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpioStructure);

	/* Output HSE clock on MCO pin (PA8) */
	RCC_MCOConfig(RCC_MCOSource_HSE);
}

volatile uint32_t clockSpeed = 0;

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

    for (;;)
    {
    	clockSpeed = SystemCoreClock;
    }

	// yup
	return 0;
}

/**
 * @brief TIM2 Timer Interrupt Handler
 */
extern "C" void TIM2_IRQHandler()
{
    // check the interrupt status;
    // if the interrupt is SET, toggle the LED
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
    }
}
