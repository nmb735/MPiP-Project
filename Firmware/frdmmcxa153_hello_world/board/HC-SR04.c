/*
 * HC-SR04.c
 *
 *  Created on: May 11, 2025
 *      Author: NMB
 */
#include "HC-SR04.h"
#include "fsl_debug_console.h"
#include <math.h>

static volatile uint32_t echoStartTime = 0;
static volatile uint32_t echoEndTime = 0;
static volatile bool measurementInProgress = false;

/* Initialize HC-SR04 sensor */
void InitHCSR04(void)
{

    /* Configure timer for HC-SR04 */
    CLOCK_EnableClock(kCLOCK_GateCTIMER2);  // Enable clock for CTIMER1 instead
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER2, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER2);    // Use high frequency clock
    
    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(HCSR04_TIMER, &config);
    CTIMER_StartTimer(HCSR04_TIMER);      // Start timer for time measurements

    /* Configure TRIG pin as digital output */
    gpio_pin_config_t trigConfig = {
        kGPIO_DigitalOutput, 0,  /* Start with logic low */
    };
    GPIO_PinInit(HCSR04_TRIG_GPIO, HCSR04_TRIG_PIN, &trigConfig);

    /* Configure ECHO pin as digital input */
    gpio_pin_config_t echoConfig = {
        kGPIO_DigitalInput, 0,
    };
    GPIO_PinInit(HCSR04_ECHO_GPIO, HCSR04_ECHO_PIN, &echoConfig);

    // Configure proper edge detection for echo pin
    //GPIO_SetPinInterruptConfig(HCSR04_ECHO_GPIO, HCSR04_ECHO_PIN, kGPIO_InterruptEitherEdge);
    // Enable interrupt flags for the pin
    //HCSR04_ECHO_GPIO->ISFR[0] = (1U << HCSR04_ECHO_PIN); // Clear any pending interrupt
    
    /* Configure GPIO interrupt for ECHO pin */
    /* Enable GPIO interrupt */
    //EnableIRQ(GPIO3_IRQn);
}

/* GPIO interrupt handler for ECHO pin */
void GPIO_IRQHandler(void)
{
        uint32_t currentTime = HCSR04_TIMER->TC;  /* Use CTIMER1 for time measurement */
    
    /* Check if this is an ECHO pin interrupt */
    if (GPIO_GpioGetInterruptFlags(HCSR04_ECHO_GPIO) & (1U << HCSR04_ECHO_PIN))
    {
        /* Rising edge - start of echo pulse */
        if (GPIO_PinRead(HCSR04_ECHO_GPIO, HCSR04_ECHO_PIN))
        {
            echoStartTime = currentTime;
            measurementInProgress = true;
        }
        /* Falling edge - end of echo pulse */
        else if (measurementInProgress)
        {
            echoEndTime = currentTime;
            measurementInProgress = false;
        }
        
        /* Clear the interrupt flag */
        GPIO_GpioClearInterruptFlags(HCSR04_ECHO_GPIO, (1U << HCSR04_ECHO_PIN));
    }
}
float TriggerMeasurement(void)
{
    float distance = 0.0f;
    uint32_t startTime, endTime, pulseDuration;
    uint32_t timeout = 0;
    
    /* Send trigger pulse */
    GPIO_PortSet(HCSR04_TRIG_GPIO, HCSR04_TRIG_PIN_MASK);
    SDK_DelayAtLeastUs(10, CLOCK_GetFreq(kCLOCK_CoreSysClk));  // 10μs trigger
    GPIO_PortClear(HCSR04_TRIG_GPIO, HCSR04_TRIG_PIN_MASK);
    
    /* Wait for echo to go high */
    timeout = 0;
    while (GPIO_PinRead(HCSR04_ECHO_GPIO, HCSR04_ECHO_PIN) == 0) {
        if (++timeout > 30000) {  // Timeout if no response
            return MAX_DISTANCE_CM;
        }
    }
    
    /* Start timing */
    startTime = HCSR04_TIMER->TC;
    
    /* Wait for echo to go low */
    timeout = 0;
    while (GPIO_PinRead(HCSR04_ECHO_GPIO, HCSR04_ECHO_PIN) == 1) {
        if (++timeout > 30000) {  // Timeout if stuck high
            return MAX_DISTANCE_CM;
        }
    }
    
    /* End timing */
    endTime = HCSR04_TIMER->TC;
    
    /* Calculate duration */
    if (endTime > startTime) {
        pulseDuration = endTime - startTime;
    } else {
        pulseDuration = (HCSR04_TIMER->MR[0] - startTime) + endTime; // Handle timer overflow
    }
    
    /* Convert to distance */
    float timeSeconds = (float)pulseDuration / CLOCK_GetCTimerClkFreq(2U);
    distance = (timeSeconds * SOUND_SPEED * 100.0f) / 2.0f;
    
    if (distance > MAX_DISTANCE_CM) {
        distance = MAX_DISTANCE_CM;
    }
    
    return distance;
}

/* Trigger a measurement and return distance in cm */
//float TriggerMeasurement(void)
//{
//    uint32_t timeoutCounter = 0;
//    float distance = 0.0f;
//    uint32_t pulseDuration = 0;
//
//    /* Reset measurement flags */
//    measurementInProgress = false;
//
//    /* Send trigger pulse */
//    GPIO_PortSet(HCSR04_TRIG_GPIO, HCSR04_TRIG_PIN_MASK);
//    SDK_DelayAtLeastUs(TRIGGER_PULSE_US, CLOCK_GetFreq(kCLOCK_CoreSysClk));
//    GPIO_PortClear(HCSR04_TRIG_GPIO, HCSR04_TRIG_PIN_MASK);
//
//    /* Wait for measurement to complete or timeout */
//    while (measurementInProgress && (timeoutCounter < 50000))
//    {
//        timeoutCounter++;
//        SDK_DelayAtLeastUs(10, CLOCK_GetFreq(kCLOCK_CoreSysClk));
//    }
//
//    /* Calculate distance if measurement was successful */
//    if (echoEndTime > echoStartTime)
//    {
//        pulseDuration = echoEndTime - echoStartTime;
//
//        /* Convert time to distance (in cm) */
//        /* Formula: distance = (time * speed of sound) / 2 */
//        /* Time needs to be converted from timer ticks to seconds */
//        float timeSeconds = (float)pulseDuration / CLOCK_GetCTimerClkFreq(2U);
//        distance = (timeSeconds * SOUND_SPEED * 100.0f) / 2.0f;
//
//        /* Limit to maximum measurable distance */
//        if (distance > MAX_DISTANCE_CM)
//        {
//            distance = MAX_DISTANCE_CM;
//        }
//    }
//    else
//    {
//        /* No echo received or measurement failed */
//        distance = MAX_DISTANCE_CM;
//    }
//
//    return distance;
//}

/* Convert distance to presence/alarm level */
alarmLevel GetPresenceLevel(float distance)
{
    if (distance <= PRESENCE_HIGH)
    {
        return ALARM_LEVEL_HIGH;
    }
    else if (distance <= PRESENCE_MEDIUM)
    {
        return ALARM_LEVEL_MEDIUM;
    }
    else if (distance <= PRESENCE_LOW)
    {
        return ALARM_LEVEL_LOW;
    }
    else
    {
        return ALARM_LEVEL_NONE;
    }
}
