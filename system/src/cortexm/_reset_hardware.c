//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include "stm32f3xx.h"

// ----------------------------------------------------------------------------

extern void __attribute__((noreturn)) NVIC_SystemReset(void);

// ----------------------------------------------------------------------------

// Forward declarations

void __reset_hardware(void);

// ----------------------------------------------------------------------------

/**
 * @brief  This is the default hardware reset routine; it can be redefined in the application for more complex applications.
 *
 * Called from _exit().
 *
 * @callgraph
 * @callergraph
 */

void __attribute__((weak,noreturn)) __reset_hardware()
{
	NVIC_SystemReset();
}

// ----------------------------------------------------------------------------
