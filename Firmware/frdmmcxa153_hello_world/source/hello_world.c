/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"

#include "fsl_clock.h"
#include "fsl_reset.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */

int main(void)
{
	const int delay = 100000;
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF("hello world.\r\n");

    // Configure RED LED pin as digital output
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput, 0,  // Start with logic low
    };
    GPIO_PinInit(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN, &led_config);
    GPIO_PinInit(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN, &led_config);
    GPIO_PinInit(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN, &led_config);
    GPIO_PinInit(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN, &led_config);
    GPIO_PinInit(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN, &led_config);
    GPIO_PinInit(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN, &led_config);
    GPIO_PinInit(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN, &led_config);

    GPIO_PortSet(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN_MASK);
    GPIO_PortSet(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN_MASK);
    GPIO_PortSet(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);
    for (volatile int i = 0; i < 1000000; i++) {}  // crude delay
    GPIO_PortClear(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);
    for (volatile int i = 0; i < 1000000; i++) {}  // crude delay

    while (1)
    {
        GPIO_PortSet(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN_MASK);
        GPIO_PortClear(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN_MASK);
        GPIO_PortClear(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN_MASK);
        GPIO_PortClear(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN_MASK);
        for (volatile int i = 0; i < delay; i++) {}  // crude delay
        GPIO_PortSet(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN_MASK);
		GPIO_PortClear(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN_MASK);
		GPIO_PortClear(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN_MASK);
		GPIO_PortClear(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN_MASK);
		for (volatile int i = 0; i < delay; i++) {}  // crude delay
        GPIO_PortSet(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN_MASK);
        GPIO_PortClear(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN_MASK);
        GPIO_PortClear(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN_MASK);
        GPIO_PortClear(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN_MASK);
        for (volatile int i = 0; i < delay; i++) {}  // crude delay
        GPIO_PortSet(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN_MASK);
		GPIO_PortClear(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN_MASK);
		GPIO_PortClear(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN_MASK);
		GPIO_PortClear(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN_MASK);
		for (volatile int i = 0; i < delay; i++) {}  // crude delay



    }
}
