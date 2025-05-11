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
#include "HC-SR04.h"

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
    bool alertFlag = false;           // Flag to determine if an alert must be set.
    alarmLevel alarm = ALARM_LEVEL_NONE; // Level of the alert.
    float counter = 0;                // Rotation counter, in degrees. Can take the values from 0-360.
    uint32_t frequencyHz = 3000;      // PWM frequency for buzzer
    alarmLevel currentAlarm = ALARM_LEVEL_NONE;

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    InitAlarm(frequencyHz);
    InitMotor();
    InitHCSR04();

    while (1)
    {
        /* Rotate one degree */
        RotateOneDegree();

        /* Wait for rotation to complete */
        //SDK_DelayAtLeastUs(100000, CLOCK_GetFreq(kCLOCK_CoreSysClk));

        /* Get current angle and print it */
        counter = GetCurrentAngle();

        /* Trigger measurement and get distance */
        float distance = TriggerMeasurement();

        /* Determine alarm level based on distance */
        alarmLevel detectedLevel = GetPresenceLevel(distance);

        /* Update alarm level if presence detected */
        if (detectedLevel > currentAlarm) {
            currentAlarm = detectedLevel;
        }

        PRINTF("Angle: %d degrees, Distance: %d cm, Alarm: %d\r\n", (int)counter, (int)distance, currentAlarm);
        
        /* Update alarm system */
        if (alarm != currentAlarm) {
            alarm = currentAlarm;
            UpdateAlarm(alarm);
        }
    }
}
