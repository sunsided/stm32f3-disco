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
    gpioStructure.GPIO_Pin = GPIO_Pin_8;
    gpioStructure.GPIO_Mode = GPIO_Mode_OUT;
    gpioStructure.GPIO_OType = GPIO_OType_PP;
    gpioStructure.GPIO_PuPd = GPIO_PuPd_UP;
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &gpioStructure);

    GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_RESET);
}

/**
 * @brief Initializes the timer.
 *
 * This function initializes the TIM2 timer.
 */
void InitializeTimer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 40000;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 500;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure);
    TIM_Cmd(TIM2, ENABLE);

    // allow timer interrupt
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
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
	gpioStructure.GPIO_Pin = GPIO_Pin_8;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF;
	gpioStructure.GPIO_OType = GPIO_OType_PP;
	gpioStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &gpioStructure);

	/* Output HSE clock on MCO pin (PA8) */
	RCC_MCOConfig(RCC_MCOSource_HSE);
}

/**
* @brief  Toggles the specified GPIO pins.
* @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
* @param  GPIO_Pin: Specifies the pins to be toggled.
*/
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    GPIOx->ODR ^= GPIO_Pin;
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
        GPIO_ToggleBits(GPIOE, GPIO_Pin_8);
    }
}
