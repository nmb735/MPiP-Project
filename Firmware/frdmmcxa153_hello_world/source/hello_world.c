/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/******************************************************************************/
/* Includes                                                                   */
/******************************************************************************/
#include "fsl_device_registers.h" /* Device Headers                           */
#include "fsl_debug_console.h"    /* Debug console                            */
#include "pin_mux.h"              /* Pins                                     */
#include "board.h"                /* Development Board                        */
#include "fsl_clock.h"            /* Clock                                    */
#include "fsl_reset.h"            /* Reset                                    */
#include <stdbool.h>              /* Standard boolean type                    */
#include "alarm.h"                /* Custom Library: Alarm System             */
#include "motor.h"                /* Custom Library: Motor Control            */
#include "HC-SR04.h"              /* Custom Library: HC-SR04 Ultrasonic Sens. */
/******************************************************************************/


/******************************************************************************/
/* Definitions                                                                */
/******************************************************************************/
/* Threshold distances for different alarm levels                             */
#define THRESHOLD_LOW    70.0f    /* Distance [cm] - Sets alarm level LOW     */
#define THRESHOLD_MEDIUM 50.0f    /* Distance [cm] - Sets alarm level MEDIUM  */
#define THRESHOLD_HIGH   10.0f    /* Distance [cm] - Sets alarm level HIGH    */
/* Angle after which alarm can decrease if no threat detected                 */
#define DECREASE_ALARM   90.0f    /* Degrees [deg] - Sector size              */
/* Base frequency for buzzer tone                                             */
#define PWM_BASE_FREQ    3000U    /* Frequency [Hz] - Buzzer base frequency   */    
/******************************************************************************/


/******************************************************************************/
/* Code                                                                       */
/******************************************************************************/
/*!
 * @brief Main function, entry point of the program. Implements the logic of a
 *       perimeter defense system using a stepper motor and an ultrasonic
 *       sensor.
 * @details The system rotates a sensor to detect intruders and triggers
 *       alarms based on distance measurements. The alarm level is
 *       adjusted based on the detected distance, and the system can
 *       decrease the alarm level if no threats are detected in a sector.
 *       The system uses a 28BYJ-48 stepper motor for rotation and an
 *       HC-SR04 ultrasonic sensor for distance measurement.
 * @return 0 on success, or a negative error code on failure.
 */
int main(void)
{
    /*-------------------------------------------------------------------------*/
    /* System State-Tracking Variables                                         */
    /*-------------------------------------------------------------------------*/
    alarmLevel alarm = ALARM_LEVEL_NONE; /* Current threat level               */
    float currentAngle = 0.0f;           /* Current sensor angle (0-360°)      */
    float startAngle = 0.0f;             /* Starting angle of current sector   */
    bool decreaseCheckDue = false;       /* Flag: 90° sector completed         */
    bool threatDetectedInSector = false; /* Flag: Threat in current sector     */
    /*-------------------------------------------------------------------------*/
    
    /*-------------------------------------------------------------------------*/
    /* Hardware Initialization                                                 */
    /*-------------------------------------------------------------------------*/
    BOARD_InitBootPins();           /* Initialize board GPIO pins              */
    BOARD_InitBootClocks();         /* Set up system clocks                    */
    BOARD_InitDebugConsole();       /* Enable debug console (UART)             */
    InitAlarm(PWM_BASE_FREQ);       /* Initialize alarm system (LEDs & buzzer) */
    InitMotor();                    /* Initialize stepper motor                */
    InitHCSR04();                   /* Initialize ultrasonic sensor            */
    ClearAlarmLevel();              /* Start with all alarms off               */
    /*-------------------------------------------------------------------------*/

    /*-------------------------------------------------------------------------*/
    /* System Startup Message                                                  */
    /*-------------------------------------------------------------------------*/
    PRINTF("|--------------------------------------|\r\n");
    PRINTF("|        PERIMETER DEFENSE SYSTEM      |\r\n");
    PRINTF("|               ACTIVATED              |\r\n");
    PRINTF("|--------------------------------------|\r\n");
    PRINTF("INFO: Initiating surveillance...\r\n");
    /*-------------------------------------------------------------------------*/

    /*-------------------------------------------------------------------------*/
    /* Main Control Loop                                                       */
    /*-------------------------------------------------------------------------*/
    while (1)
    {
        /* STEP 1: Rotate the sensor by one degree */
        RotateOneDegree();
        /* STEP 1.1: Get current position */
        currentAngle = GetCurrentAngle();


        /* STEP 2: Measure distance to detect potential threats */
        float distance = TriggerMeasurement();
        alarmLevel currentThreat = ALARM_LEVEL_NONE;
        

        /* STEP 3: Check if we've completed a sector rotation */
        float angleDifference = currentAngle - startAngle;
        if (angleDifference < 0.0f) {
            angleDifference += 360.0f;  /* Wrap Around (359° to 0°) */
        }
        /* STEP 3.1: Mark sector completion when angle difference exceeds threshold */
        if (angleDifference >= DECREASE_ALARM) {
            decreaseCheckDue = true;
            threatDetectedInSector = false;
        }


        /* STEP 4: Determine threat level based on measured distance */
        if (distance < THRESHOLD_HIGH) {
            currentThreat = ALARM_LEVEL_HIGH;
        } else if (distance < THRESHOLD_MEDIUM) {
            currentThreat = ALARM_LEVEL_MEDIUM;
        } else if (distance < THRESHOLD_LOW) {
            currentThreat = ALARM_LEVEL_LOW;
        }
        

        /* STEP 5: React to threat detection */
        if (currentThreat > ALARM_LEVEL_NONE) {
            /* Mark that we've detected a threat in this sector */
            threatDetectedInSector = true;
            
            /* Reset sector tracking - start a new sector from current position */
            startAngle = currentAngle;
            decreaseCheckDue = false;
            
            /* Update alarm if a higher threat level is detected */
            if (currentThreat > alarm) {
                alarm = currentThreat;
                
                /* Alert message based on threat severity */
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
        

        /* STEP 6: Process sector completion */
        if (decreaseCheckDue) {
            /* Check if we should decrease the alarm level (after sector rotation with no threats) */
            if ((alarm > ALARM_LEVEL_NONE) && (!threatDetectedInSector)) {
                /* No threats detected in this sector - decrease alarm level */
                alarm--;
                PRINTF("INFO: No threats detected for 90 degrees - Decreasing alert level to %d\r\n", alarm);
            } else if (threatDetectedInSector) {
                /* Threats detected - maintain current alarm level */
                PRINTF("INFO: Threats detected in sector - Maintaining alert level %d\r\n", alarm);
            }
            
            /* Reset tracking variables for the next sector */
            threatDetectedInSector = false;
            decreaseCheckDue = false;
            startAngle = currentAngle; 
        }
        

        /* STEP 7: Update physical alarm indicators (LEDs & buzzer) */
        UpdateAlarm(alarm);
    }
    /*-------------------------------------------------------------------------*/
}
/******************************************************************************/