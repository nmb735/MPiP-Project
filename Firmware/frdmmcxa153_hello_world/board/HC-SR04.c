/*******************************************************************************
 * HC-SR04 Ultrasonic Distance Sensor Library
 *
 * File: HC-SR04.c
 * Description: Implementation file for HC-SR04 ultrasonic sensor interface.
 *              Provides functions to initialize the sensor, measure distances,
 *              and interpret distance measurements as presence levels.
 *
 * Created on: May 11, 2025
 * Author: Nedal M. Benelmekki
 ******************************************************************************/


/******************************************************************************/
/* Includes                                                                   */
/******************************************************************************/
#include "HC-SR04.h"             /* HC-SR04 interface definitions             */
/******************************************************************************/


/******************************************************************************/
/* Private Variables                                                          */
/******************************************************************************/
/* Variables for interrupt-based timing (unused in polling implementation)    */
static volatile uint32_t echoStartTime = 0;
static volatile uint32_t echoEndTime = 0;
static volatile bool measurementInProgress = false;
/******************************************************************************/


/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
/**
 * @brief Initialize the HC-SR04 ultrasonic sensor
 * 
 * Configures the timer and GPIO pins required for the HC-SR04 sensor.
 * Sets up the trigger pin as output and echo pin as input.
 */
void InitHCSR04(void)
{
    /* Configure timer for HC-SR04 pulse measurements */
    CLOCK_EnableClock(kCLOCK_GateCTIMER2); 
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER2, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER2);
    
    /* Initialize timer with default configuration */
    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(HCSR04_TIMER, &config);
    CTIMER_StartTimer(HCSR04_TIMER);

    /* Configure TRIG pin as digital output (initially low) */
    gpio_pin_config_t trigConfig = {
        kGPIO_DigitalOutput, 0,
    };
    GPIO_PinInit(HCSR04_TRIG_GPIO, HCSR04_TRIG_PIN, &trigConfig);

    /* Configure ECHO pin as digital input */
    gpio_pin_config_t echoConfig = {
        kGPIO_DigitalInput, 0,
    };
    GPIO_PinInit(HCSR04_ECHO_GPIO, HCSR04_ECHO_PIN, &echoConfig);

    /* Note: Interrupt-based configuration is disabled in this implementation */
    /* The system uses polling-based measurement instead */
}

/**
 * @brief GPIO interrupt handler for ECHO pin (unused in polling implementation)
 * 
 * This handler processes interrupts from the ECHO pin to measure the pulse duration.
 * It records the start time on rising edge and end time on falling edge.
 */
void GPIO_IRQHandler(void)
{
    uint32_t currentTime = HCSR04_TIMER->TC;
    
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

/**
 * @brief Trigger a distance measurement and return the result using polling
 * 
 * Sends a trigger pulse to the HC-SR04 sensor, then uses polling to measure
 * the echo pulse duration and calculate the distance.
 * 
 * @return Distance in centimeters (cm). Returns MAX_DISTANCE_CM if no echo received.
 */
float TriggerMeasurement(void)
{
    float distance = 0.0f;
    uint32_t startTime, endTime, pulseDuration;
    uint32_t timeout = 0;
    
    /* Small delay before trigger to let system stabilize */
    SDK_DelayAtLeastUs(5000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    
    /* Send trigger pulse */
    GPIO_PortSet(HCSR04_TRIG_GPIO, HCSR04_TRIG_PIN_MASK);
    SDK_DelayAtLeastUs(15, CLOCK_GetFreq(kCLOCK_CoreSysClk)); /* 15μs trigger pulse */
    GPIO_PortClear(HCSR04_TRIG_GPIO, HCSR04_TRIG_PIN_MASK);
    
    /* Small delay after trigger before measuring echo */
    SDK_DelayAtLeastUs(60, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    
    /* Wait for echo pin to go high (pulse start) */
    timeout = 0;
    while (GPIO_PinRead(HCSR04_ECHO_GPIO, HCSR04_ECHO_PIN) == 0) {
        if (++timeout > 60000) {  /* Timeout if no response */
            return MAX_DISTANCE_CM;
        }
        SDK_DelayAtLeastUs(1, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    }
    
    /* Record start time of echo pulse */
    startTime = HCSR04_TIMER->TC;
    
    /* Wait for echo pin to go low (pulse end) */
    timeout = 0;
    while (GPIO_PinRead(HCSR04_ECHO_GPIO, HCSR04_ECHO_PIN) == 1) {
        if (++timeout > 60000) {  /* Timeout if stuck high */
            return MAX_DISTANCE_CM;
        }
        SDK_DelayAtLeastUs(1, CLOCK_GetFreq(kCLOCK_CoreSysClk)); 
    }
    
    /* Record end time of echo pulse */
    endTime = HCSR04_TIMER->TC;
    
    /* Calculate pulse duration, handling timer overflow */
    if (endTime > startTime) {
        pulseDuration = endTime - startTime;
    } else {
        pulseDuration = (HCSR04_TIMER->MR[0] - startTime) + endTime;
    }
    
    /* Convert pulse duration to distance using speed of sound */
    /* Formula: distance = (time * speed of sound) / 2 */
    float timeSeconds = (float)pulseDuration / CLOCK_GetCTimerClkFreq(2U);
    distance = (timeSeconds * SOUND_SPEED * 100.0f) / 2.0f;
    
    /* Limit to maximum measurable distance */
    if (distance > MAX_DISTANCE_CM) {
        distance = MAX_DISTANCE_CM;
    }
    
    return distance;
}

/**
 * @brief Convert distance to presence/alarm level
 * 
 * Interprets a measured distance into an appropriate alarm level
 * based on predefined distance thresholds.
 * 
 * @param distance The measured distance in centimeters
 * @return The corresponding alarm level (NONE, LOW, MEDIUM, or HIGH)
 */
alarmLevel GetPresenceLevel(float distance)
{
    if (distance <= PRESENCE_HIGH){
        return ALARM_LEVEL_HIGH;
    }
    else if (distance <= PRESENCE_MEDIUM){
        return ALARM_LEVEL_MEDIUM;
    }
    else if (distance <= PRESENCE_LOW){
        return ALARM_LEVEL_LOW;
    }
    else{
        return ALARM_LEVEL_NONE;
    }
}
/******************************************************************************/
