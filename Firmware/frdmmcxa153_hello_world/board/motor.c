/*******************************************************************************
 * Stepper Motor Control Library
 *
 * File: motor.c
 * Description: Implementation of stepper motor control interface. Handles
 *              precise control of a 28BYJ-48 stepper motor using the
 *              ULN2003 driver board with four control signals.
 *
 * Created on: May 10, 2025
 * Author: Nedal M. Benelmekki
 ******************************************************************************/


/******************************************************************************/
/* Includes                                                                   */
/******************************************************************************/
#include "motor.h"
/******************************************************************************/


/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
/**
 * @brief Initialize motor control with timer interrupts
 * 
 * Sets up the GPIO pins for motor control and initializes the timer that
 * will trigger motor steps at a regular interval.
 */
void InitMotor(void)
{
    /* Configure motor output pins as digital outputs */
    gpio_pin_config_t motorConfig = {
        kGPIO_DigitalOutput, 0,
    };

    /* Initialize each control pin */
    GPIO_PinInit(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN, &motorConfig);
    GPIO_PinInit(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN, &motorConfig);
    GPIO_PinInit(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN, &motorConfig);
    GPIO_PinInit(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN, &motorConfig);

    /* Use CTIMER0 for motor (assuming CTIMER1 is used for alarm) */
    CLOCK_EnableClock(kCLOCK_GateCTIMER0);
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER0, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER0);

    /* Configure CTIMER0 as a timer with interrupts */
    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER0, &config);

    /* Set up timer match for motor step rate */
    uint32_t timerFreq = CLOCK_GetCTimerClkFreq(0U);
    uint32_t matchValue = timerFreq / MOTOR_STEP_RATE;

    /* Configure match register to trigger interrupts */
    ctimer_match_config_t matchConfig;
    matchConfig.enableCounterReset = true;
    matchConfig.enableCounterStop = false;
    matchConfig.matchValue = matchValue;
    matchConfig.outControl = kCTIMER_Output_NoAction;
    matchConfig.outPinInitState = false;
    matchConfig.enableInterrupt = true;
    CTIMER_SetupMatch(CTIMER0, kCTIMER_Match_0, &matchConfig);
    
    /* Enable timer interrupts */
    EnableIRQ(CTIMER0_IRQn);
    CTIMER_EnableInterrupts(CTIMER0, CTIMER_IR_MR0INT_MASK);
}


/**
 * @brief Timer interrupt handler for motor stepping
 * 
 * This function is called when the timer interrupt fires,
 * steps the motor once if there are remaining steps to take.
 */
void CTIMER0_IRQHandler(void) 
{
    /* Clear the interrupt flag */
    CTIMER_ClearStatusFlags(CTIMER0, CTIMER_IR_MR0INT_MASK);
    
    /* If we have remaining steps to take */
    if (motorMoving && remainingSteps > 0) {
        StepMotorOnce();
        remainingSteps--;
        
        if (remainingSteps <= 0) {
            motorMoving = false;
            /* Turn off all coils to save power when not moving */
            GPIO_PortClear(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN_MASK);
        }
    }
}

/**
 * @brief Execute a single step of the stepper motor
 * 
 * Implements the four-step sequence for the stepper motor,
 * which energizes the coils in the proper sequence.
 *
 * @return The current angle after stepping
 */
float StepMotorOnce(void)
{
    /* Four-step sequence for 28BYJ-48 stepper motor with ULN2003 driver */
    switch (currentStep) {
        case 0:  // Step 1
            GPIO_PortSet(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN_MASK);
            break;
            
        case 1:  // Step 2
            GPIO_PortClear(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN_MASK);
            GPIO_PortSet(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN_MASK);
            break;
            
        case 2:  // Step 3
            GPIO_PortClear(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN_MASK);
            GPIO_PortSet(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN_MASK);
            break;
            
        case 3:  // Step 4
            GPIO_PortClear(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN_MASK);
            GPIO_PortSet(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN_MASK);
            break;
    }
    
    /* Update the step counter and wrap around at 4 */
    currentStep = (currentStep + 1) % STEP_SEQUENCE_COUNT;

    /* Update the angle and wrap around at 360 degrees */
    currentAngle += (((MOTOR_STEP_ANGLE) / (GEAR_RATIO))*2.0f);
    //currentAngle += ACTUAL_STEP_ANGLE;
    if (currentAngle >= 360.0f) {
        currentAngle -= 360.0f;
    }

    return currentAngle;
}

/**
 * @brief Rotate the motor by the specified number of degrees
 * 
 * @param degrees Angle to rotate in degrees
 */
void RotateDegrees(float degrees)
{
    /* Calculate the number of steps needed to rotate by the specified angle */
    int steps = (int)((fabs(degrees) * STEPS_PER_DEGREE) + 0.5f);
    //int steps = (int)(degrees * STEPS_PER_DEGREE);

    if (!motorMoving) {
        remainingSteps = steps;
        motorMoving = true;

        /* Start the timer if not already running */
        CTIMER_StartTimer(CTIMER0);
    }
}

/**
 * @brief Rotate exactly one degree
 */
void RotateOneDegree(void)
{
    RotateDegrees(1.0f);
}

/**
 * @brief Get the current angle of the motor
 * 
 * @return Current angle in degrees (0-360)
 */
float GetCurrentAngle(void)
{
    return currentAngle;
}
