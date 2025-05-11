/*******************************************************************************
 * HC-SR04 Ultrasonic Distance Sensor Library
 *
 * File: HC-SR04.h
 * Description: Header file for HC-SR04 ultrasonic sensor interface. Provides
 *              functionality to measure distances using the HC-SR04 sensor,
 *              including initialization, measurement, and distance interpretation.
 *
 * Created on: May 11, 2025
 * Author: Nedal M. Benelmekki
 ******************************************************************************/

#ifndef _HC_SR04_H_
#define _HC_SR04_H_

/******************************************************************************/
/* Includes                                                                   */
/******************************************************************************/
#include "fsl_device_registers.h" /* Device Headers                           */
#include "board.h"                /* Board definition                         */
#include "fsl_gpio.h"             /* GPIO control                             */
#include "fsl_ctimer.h"           /* CTIMER for echo pulse measurement        */
#include "fsl_debug_console.h"   /* Debug console for logging                 */
#include <math.h>                /* Math functions                            */
#include "alarm.h"                /* For alarm level definitions              */
/******************************************************************************/


/******************************************************************************/
/* HC-SR04 Pin Definitions                                                    */
/******************************************************************************/
/* Trigger pin configuration                                                  */
#define HCSR04_TRIG_GPIO      BOARD_INITPINS_HCSR04_TRIG_GPIO 
#define HCSR04_TRIG_PIN       BOARD_INITPINS_HCSR04_TRIG_GPIO_PIN
#define HCSR04_TRIG_PIN_MASK  BOARD_INITPINS_HCSR04_TRIG_GPIO_PIN_MASK
/* Echo pin configuration                                                     */
#define HCSR04_ECHO_GPIO      BOARD_INITPINS_HCSR04_ECHO_GPIO
#define HCSR04_ECHO_PIN       BOARD_INITPINS_HCSR04_ECHO_GPIO_PIN
#define HCSR04_ECHO_PIN_MASK  BOARD_INITPINS_HCSR04_ECHO_GPIO_PIN_MASK
/* Timer used for pulse measurement                                           */
#define HCSR04_TIMER          CTIMER2
/******************************************************************************/


/******************************************************************************/
/* HC-SR04 Sensor Parameters and Constants                                    */
/******************************************************************************/
/* Constants for distance calculation                                         */
#define SOUND_SPEED          343.0f  /* Speed of sound in m/s at 20°C         */
#define MAX_DISTANCE_CM      400     /* Maximum distance in cm                */
#define TRIGGER_PULSE_US     10      /* Trigger pulse width in microseconds   */
/* Presence detection thresholds (in cm)                                      */
#define PRESENCE_NONE        400.0f  /* No presence detected (beyond 4m)      */
#define PRESENCE_LOW         300.0f  /* Low presence detected (3-4m)          */
#define PRESENCE_MEDIUM      150.0f  /* Medium presence detected (1.5-3m)     */
#define PRESENCE_HIGH        50.0f   /* High presence detected (<0.5m)        */
/******************************************************************************/


/******************************************************************************/
/* Function Prototypes                                                        */
/******************************************************************************/
/**
 * @brief Initialize the HC-SR04 ultrasonic sensor
 * 
 * Configures the timer and GPIO pins required for the HC-SR04 sensor.
 * Sets up the trigger pin as output and echo pin as input.
 */
void InitHCSR04(void);

/**
 * @brief Trigger a distance measurement and return the result
 * 
 * Sends a trigger pulse to the HC-SR04 sensor and measures the echo pulse
 * duration to calculate the distance to the nearest object.
 * 
 * @return Distance in centimeters (cm). Returns MAX_DISTANCE_CM if no echo received.
 */
float TriggerMeasurement(void);

/**
 * @brief Convert distance to presence/alarm level
 * 
 * Interprets a measured distance into an appropriate alarm level
 * based on predefined distance thresholds.
 * 
 * @param distance The measured distance in centimeters
 * @return The corresponding alarm level (NONE, LOW, MEDIUM, or HIGH)
 */
alarmLevel GetPresenceLevel(float distance);
/******************************************************************************/

#endif /* _HC_SR04_H_ */
