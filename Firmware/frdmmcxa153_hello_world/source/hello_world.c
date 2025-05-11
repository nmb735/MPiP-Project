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
#define THRESHOLD_LOW 70.0f
#define THRESHOLD_MEDIUM 50.0f
#define THRESHOLD_HIGH 10.0f

#define DECREASE_ALARM 90.0f

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
    // Simplified system variables
    alarmLevel alarm = ALARM_LEVEL_NONE;    // Current threat level
    float currentAngle = 0.0f;              // Current sensor angle
    float startAngle = 0.0f;                // Starting angle for decreasing alarm
    bool decreaseCheckDue = false;          // Flag to check if we should decrease alarm
    bool threatDetectedInSector = false;    // Consolidated detection tracking
    
    // Initialize hardware
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    InitAlarm(3000);  // 3kHz alarm frequency
    InitMotor();
    InitHCSR04();
    ClearAlarmLevel();

    // System startup
    PRINTF("|--------------------------------------|\r\n");
    PRINTF("|        PERIMETER DEFENSE SYSTEM      |\r\n");
    PRINTF("|               ACTIVATED              |\r\n");
    PRINTF("|--------------------------------------|\r\n");
    PRINTF("INFO: Initiating surveillance...\r\n");

    while (1)
    {
        // Rotate sensor one degree
        RotateOneDegree();
        currentAngle = GetCurrentAngle();

        // Measure distance and assess threat
        float distance = TriggerMeasurement();
        alarmLevel currentThreat = ALARM_LEVEL_NONE;
        
        // Check if we've rotated 90 degrees since last threat
        float angleDifference = currentAngle - startAngle;
        if (angleDifference < 0.0f) {
            angleDifference += 360.0f;  // Handle wraparound
        }
        
        // Mark sector completion
        if (angleDifference >= DECREASE_ALARM) {
            decreaseCheckDue = true;
            threatDetectedInSector = false;
        }

        // Determine threat level based on distance
        if (distance < THRESHOLD_HIGH) {
            currentThreat = ALARM_LEVEL_HIGH;
        } else if (distance < THRESHOLD_MEDIUM) {
            currentThreat = ALARM_LEVEL_MEDIUM;
        } else if (distance < THRESHOLD_LOW) {
            currentThreat = ALARM_LEVEL_LOW;
        }
        
        // React to threat detection
        if (currentThreat > ALARM_LEVEL_NONE) {
            // Record threat for this sector
            threatDetectedInSector = true;
            
            // Reset sector tracking
            startAngle = currentAngle;
            decreaseCheckDue = false;
            
            // Update alarm if higher threat
            if (currentThreat > alarm) {
                alarm = currentThreat;
                switch (currentThreat) {
					case ALARM_LEVEL_HIGH:
						PRINTF("CRITICAL ALERT: Intruder detected at %d [deg] - DISTANCE: %dcm \r\n",
							(int)currentAngle, (int)distance);
						break;
					case ALARM_LEVEL_MEDIUM:
						PRINTF("WARNING: Proximity alert at %d [deg] - DISTANCE: %dcm \r\n",
							(int)currentAngle, (int)distance);
						break;
					case ALARM_LEVEL_LOW:
						PRINTF("INFO: Movement detected at %d [deg] - DISTANCE: %dcm\r\n",
							(int)currentAngle, (int)distance);
						break;
				}
            }
        }
        
        // Check if we should decrease the alarm level (after 90° with no threats)
        if (decreaseCheckDue) {
            // Check if we should decrease the alarm level (after 90° with no threats)
            if (alarm > ALARM_LEVEL_NONE && !threatDetectedInSector) {
                alarm--;
                PRINTF("INFO: No threats detected for 90 degrees - Decreasing alert level to %d\r\n", alarm);
            } else if (threatDetectedInSector) {
                PRINTF("INFO: Threats detected in sector - Maintaining alert level %d\r\n", alarm);
            }
            
            // Always reset tracking variables when sector completes
            threatDetectedInSector = false;
            decreaseCheckDue = false;
            startAngle = currentAngle; // Reset for next 90° sector
        }
        
        UpdateAlarm(alarm);

        // Alert messages based on threat level
//        if ((currentThreat > ALARM_LEVEL_NONE)) {
//            switch (currentThreat) {
//                case ALARM_LEVEL_HIGH:
//                    PRINTF("CRITICAL ALERT: Intruder detected at %d [deg] - DISTANCE: %dcm \r\n",
//                        (int)currentAngle, (int)distance);
//                    break;
//                case ALARM_LEVEL_MEDIUM:
//                    PRINTF("WARNING: Proximity alert at %d [deg] - DISTANCE: %dcm \r\n",
//                        (int)currentAngle, (int)distance);
//                    break;
//                case ALARM_LEVEL_LOW:
//                    PRINTF("INFO: Movement detected at %d [deg] - DISTANCE: %dcm\r\n",
//                        (int)currentAngle, (int)distance);
//                    break;
//            }
//        }

        // Periodic status at cardinal directions
//        if ((int)currentAngle == 0 || (int)currentAngle == 90 ||
//            (int)currentAngle == 180 || (int)currentAngle == 270) {
//            PRINTF("INFO: Position: %d [deg] | Range: %dcm | Alert Level: %d\r\n",
//                (int)currentAngle, (int)distance, alarm);
//        }
    }
}
