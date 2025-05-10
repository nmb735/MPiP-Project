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
#include "alarm.h"
#include "motor.h"

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
	bool alertFlag = false; // Flag to determine if an alert must be set.
	alarmLevel alarm = ALARM_LEVEL_NONE; // Level of the alert.
	uint16_t counter = 0; // Rotation counter, in degrees. Can take the values from 0-360.

	const int delay = 10000000;

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    InitAlarm();

    while (1)
    {
		if (alarm == ALARM_LEVEL_HIGH)
			alarm = ALARM_LEVEL_NONE;
		else
			alarm += 1;
		UpdateAlarm(alarm);
		for(volatile int i = 0; i < delay; i++){}
    }
}
