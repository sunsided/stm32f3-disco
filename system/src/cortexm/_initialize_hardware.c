//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include "stm32f3xx.h"
#include "_system_clock.h"

// ----------------------------------------------------------------------------

// Forward declarations.

void
__initialize_hardware_early(void);

void
__initialize_hardware(void);

// ----------------------------------------------------------------------------

// This is the early hardware initialisation routine, it can be
// redefined in the application for more complex cases that
// require early inits (before BSS init).
//
// Called early from _start(), right before data & bss init.
//
// After Reset the Cortex-M processor is in Thread mode,
// priority is Privileged, and the Stack is set to Main.

void
__attribute__((weak))
__initialize_hardware_early(void)
{
  // Call the CSMSIS system initialisation routine.
  SystemInit();

  // The current version of SystemInit() leaves the value of the clock
  // in a RAM variable (SystemCoreClock), which will be cleared shortly,
  // so it needs to be recomputed after the RAM initialisations
  // are completed.

#if defined(OS_INCLUDE_STARTUP_INIT_FP) || (defined (__VFP_FP__) && !defined (__SOFTFP__))

  // Normally FP init is done by SystemInit(). In case this is not done
  // there, it is possible to force its inclusion by defining
  // OS_INCLUDE_STARTUP_INIT_FP.

  // Enable the Cortex-M4 FPU only when -mfloat-abi=hard.
  // Code taken from Section 7.1, Cortex-M4 TRM (DDI0439C)

  // Set bits 20-23 to enable CP10 and CP11 coprocessor
  SCB->CPACR |= (0xF << 20);

#endif // (__VFP_FP__) && !(__SOFTFP__)
}

// This is the second hardware initialisation routine, it can be
// redefined in the application for more complex cases that
// require custom inits (before constructors), otherwise these can
// be done in main().
//
// Called from _start(), right after data & bss init, before
// constructors.

void
__attribute__((weak))
__initialize_hardware(void)
{
  /* STM32F3xx HAL library initialization:
	   - Configure the Flash prefetch
	   - Systick timer is configured by default as source of time base, but user
		 can eventually implement his proper time base source (a general purpose
		 timer for example or other time source), keeping in mind that Time base
		 duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
		 handled in milliseconds basis.
	   - Set NVIC Group Priority to 4
	   - Low Level Initialization
	 */
	HAL_Init();

	// Initialize the system clock.
	SystemClock_Config();

	// Call the CSMSIS system clock routine to store the clock frequency
	// in the SystemCoreClock global RAM location.
	SystemCoreClockUpdate();
}

// ----------------------------------------------------------------------------
