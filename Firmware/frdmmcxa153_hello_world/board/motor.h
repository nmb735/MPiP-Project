/*******************************************************************************
 * Stepper Motor Control Library
 *
 * File: motor.h
 * Description: Header file for stepper motor control interface. Provides
 *              functionality for precise angular control of a 28BYJ-48 stepper
 *              motor including initialization, stepping and rotation control.
 *
 * Created on: May 10, 2025
 * Author: NMB
 ******************************************************************************/

#ifndef MOTOR_H_
#define MOTOR_H_

/******************************************************************************/
/* Includes                                                                   */
/******************************************************************************/
/* SDK Includes                                                               */
#include "fsl_device_registers.h"     /* Device Headers                       */
#include "fsl_debug_console.h"        /* Debug console                        */
#include "fsl_gpio.h"                 /* GPIO control                         */
#include "fsl_ctimer.h"               /* CTIMER for motor timing              */
#include "fsl_clock.h"                /* Clock control                        */
#include "fsl_reset.h"                /* Reset control                        */
/* Board configuration                                                        */
#include "pin_mux.h"                  /* Pin mux configuration                */
#include "board.h"                    /* Board support                        */
#include "alarm.h"                    /* Alarm system integration             */
/* Standard libraries                                                         */
#include <stdbool.h>                  /* Boolean type                         */
#include <math.h>                      /* Math functions                      */
/******************************************************************************/


/******************************************************************************/
/* Constants and Definitions                                                  */
/******************************************************************************/
/* 28BYJ-48 Stepper Motor Characteristics */
#define MOTOR_STEP_ANGLE 5.625f       /* Base step angle in degrees         */
#define GEAR_RATIO 64.0f              /* Internal gear reduction ratio      */
#define STEP_SEQUENCE_COUNT 4         /* Steps in one phase cycle           */
#define ACTUAL_STEP_ANGLE (MOTOR_STEP_ANGLE / (GEAR_RATIO * STEP_SEQUENCE_COUNT))
#define STEPS_PER_DEGREE (1.0f / ACTUAL_STEP_ANGLE)
#define MOTOR_STEP_RATE 10            /* Default step rate in Hz            */
/******************************************************************************/


/******************************************************************************/
/* Private Variables                                                          */
/******************************************************************************/
/* Global variables to track motor state */
static float currentAngle = 0.0f;     /* Current angle in degrees (0-360)    */
static uint8_t currentStep = 0;       /* Current step in the sequence (0-3)  */
static bool motorMoving = false;      /* True when motor is in motion        */
static int remainingSteps = 0;        /* Steps remaining in current motion   */
/******************************************************************************/


/******************************************************************************/
/* Motor Control Function Prototypes                                          */
/******************************************************************************/
/**
 * @brief Initialize stepper motor control hardware and settings
 * 
 * Configures motor GPIO pins, timer for step control, and interrupt
 * handling for the stepper motor.
 */
void InitMotor(void);

/**
 * @brief Execute a single step of the stepper motor
 * 
 * @return Current angle after the step
 */
float StepMotorOnce(void);

/**
 * @brief Rotate the motor by the specified number of degrees
 * 
 * @param degrees Angle to rotate in degrees (positive = clockwise)
 */
void RotateDegrees(float degrees);

/**
 * @brief Convenience function to rotate exactly one degree
 */
void RotateOneDegree(void);

/**
 * @brief Get the current angle of the motor
 * 
 * @return Current angle in degrees (0-360)
 */
float GetCurrentAngle(void);
/******************************************************************************/

#endif /* MOTOR_H_ */