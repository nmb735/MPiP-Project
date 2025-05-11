#include "motor.h"
#include <math.h>

/* Global variables to track motor state */
static float currentAngle = 0.0f;
static uint8_t currentStep = 0;
static bool motorMoving = false;
static int remainingSteps = 0;

/* Constants for 28BYJ-48 stepper motor */
#define MOTOR_STEP_ANGLE 5.625f        /* Base step angle */
#define GEAR_RATIO 64.0f               /* Internal gear reduction */
#define STEP_SEQUENCE_COUNT 4          /* 4-step sequence (half-stepping) */
#define ACTUAL_STEP_ANGLE (MOTOR_STEP_ANGLE / (GEAR_RATIO * STEP_SEQUENCE_COUNT))
#define STEPS_PER_DEGREE (1.0f / ACTUAL_STEP_ANGLE)

/* Initialize motor control with timer */
void InitMotor(void)
{
    /* Configure motor output pins as digital outputs */
    gpio_pin_config_t motorConfig = {
        kGPIO_DigitalOutput, 0,  /* Start with logic low */
    };
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
    uint32_t timerFreq = CLOCK_GetCTimerClkFreq(1U);
    uint32_t matchValue = timerFreq / 10; /* 10Hz step rate - adjust for your needs */

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

/* Timer interrupt handler for motor stepping */
void CTIMER0_IRQHandler(void) 
{
    /* Clear the interrupt flag */
    CTIMER_ClearStatusFlags(CTIMER0, CTIMER_IR_MR0INT_MASK);
    
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

/* Execute a single step based on current position */
float StepMotorOnce(void)
{
    switch (currentStep) {
        case 0:
            GPIO_PortSet(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN_MASK);
            break;
        case 1:
            GPIO_PortSet(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN_MASK);
            break;
        case 2:
            GPIO_PortSet(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN_MASK);
            break;
        case 3:
            GPIO_PortSet(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_GPIO_PIN_MASK);
            GPIO_PortClear(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_GPIO_PIN_MASK);
            break;
    }
    
    currentStep = (currentStep + 1) % 4;
    //currentAngle += ACTUAL_STEP_ANGLE;
    currentAngle += (((MOTOR_STEP_ANGLE) / (GEAR_RATIO))*2.0f);
    if (currentAngle >= 360.0f) {
        currentAngle -= 360.0f;
    }

    return currentAngle;
}

/* Rotate by a specific number of degrees */
void RotateDegrees(float degrees)
{
    int steps = (int)((fabs(degrees) * STEPS_PER_DEGREE) + 0.5f);
    if (!motorMoving) {
        remainingSteps = steps;
        motorMoving = true;

        /* Start the timer if not already running */
        CTIMER_StartTimer(CTIMER0);
    }
}

//* Rotate by exactly one degree */
void RotateOneDegree(void)
{
    RotateDegrees(1.0f);
}

float GetCurrentAngle(void)
{
    return currentAngle;
}
