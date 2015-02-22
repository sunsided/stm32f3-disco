#pragma once
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

	#include <stm32f3xx.h>
	#include <stm32f3xx_hal.h>

	#include "usbd_desc.h"
	#include <usbd_hid.h>

#ifdef __cplusplus
}
#endif


/* It is possible to remap the USB interrupts (USB_LP and USB_WKUP) on interrupt 
   lines 75 and 76.
   Uncomment the line below to select your USB Interrupt Line */

/* #define USE_USB_INTERRUPT_DEFAULT   1 */
#define USE_USB_INTERRUPT_REMAPPED        1

#if !defined (USE_USB_INTERRUPT_DEFAULT) && !defined (USE_USB_INTERRUPT_REMAPPED)
 #error "Missing define Please Define Your Interrupt Mode By UnComment Line in main.h file"
#endif

#endif /* __MAIN_H */
