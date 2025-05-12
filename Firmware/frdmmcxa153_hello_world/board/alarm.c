/*******************************************************************************
 * Alarm System Library
 *
 * File: alarm.c
 * Description: Implementation file for alarm system interface. Controls LEDs
 *              and buzzer to provide visual and audible alerts based on 
 *              configured threat levels.
 *
 * Created on: May 10, 2025
 * Author: Nedal M. Benelmekki
 ******************************************************************************/


/******************************************************************************/
/* Includes                                                                   */
/******************************************************************************/
#include "alarm.h"
/******************************************************************************/


/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
/**
 * @brief Initialize the alarm system (LEDs and buzzer)
 * 
 * This function initializes the GPIO pins for LEDs and configures the CTIMER
 * for PWM generation to drive the buzzer. The buzzer is initially disabled.
 *
 * @param frequencyHz Base frequency for buzzer in Hz
 */
void InitAlarm(uint32_t frequencyHz){

	/* Configure GPIO pins for LEDs */
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput, 0,  // Start with logic low
    };

	/* Initialize each LED GPIO */
    GPIO_PinInit(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN, &led_config);
    GPIO_PinInit(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN, &led_config);
    GPIO_PinInit(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN, &led_config);

    /* Turn on green LED (system active indicator - Control LED) */
    GPIO_PortSet(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);

	/* Initialize CTIMER for buzzer PWM generation */
    ctimer_config_t config;
    ctimer_match_config_t matchConfig;

	/* Enable clock for GPIO and CTIMER */
    CLOCK_EnableClock(kCLOCK_GateGPIO3);
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER1, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER1);

	/* Initialize CTIMER with default configuration */
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER, &config);

    /* Configure PWM for buzzer */
    uint32_t pwmFreq = frequencyHz;
    uint32_t matchValue = CTIMER_CLK_FREQ / pwmFreq;

    /* Configure match register for PWM period */
    matchConfig.enableCounterReset = true;
    matchConfig.enableCounterStop = false;
    matchConfig.matchValue = matchValue;
    matchConfig.outControl = kCTIMER_Output_NoAction;
    matchConfig.outPinInitState = false;
    matchConfig.enableInterrupt = false;
    CTIMER_SetupMatch(CTIMER, CTIMER_MAT_PWM_PERIOD_CHANNEL, &matchConfig);
    
    /* Start the timer */
    CTIMER_StartTimer(CTIMER);
    
    /* Set the alarm to the default value */
    ClearAlarmLevel();
}

/**
 * @brief Update alarm indicators based on the specified threat level
 *
 * @param level Alarm level to set (from alarmLevel enum)
 */
void UpdateAlarm(alarmLevel level)
{
    switch (level) {
        case ALARM_LEVEL_HIGH:
            SetAlarmLevelHigh();
            break;
            
        case ALARM_LEVEL_MEDIUM:
            SetAlarmLevelMedium();
            break;
            
        case ALARM_LEVEL_LOW:
            SetAlarmLevelLow();
            break;
            
        case ALARM_LEVEL_NONE:
        default:
            ClearAlarmLevel();
            break;
    }
}

/**
 * @brief Set alarm to low level (green LED only)
 */
void SetAlarmLevelLow(){
    GPIO_PortClear(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);
    GPIO_PortSet(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN_MASK);
    DisableBuzzer();
}

/**
 * @brief Set alarm to medium level (yellow LED and slow buzzer)
 */
void SetAlarmLevelMedium(){
    GPIO_PortClear(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);
    GPIO_PortSet(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN_MASK);
	SetBuzzerIntensity(ALARM_FREQ_MEDIUM);
}

/**
 * @brief Set alarm to high level (red LED and fast buzzer)
 */
void SetAlarmLevelHigh(){
    GPIO_PortSet(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN_MASK);
    GPIO_PortSet(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);
	SetBuzzerIntensity(ALARM_FREQ_HIGH);
}

/**
 * @brief Clear all alarms (normal state)
 */
void ClearAlarmLevel(){
    GPIO_PortClear(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);
    GPIO_PortSet(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);
	DisableBuzzer();
}

/**
 * @brief Set buzzer intensity via PWM duty cycle
 *
 * @param dutyCycle PWM duty cycle percentage (0-100)
 */
void SetBuzzerIntensity(uint32_t frequency) {

    
    /*Calculate match value based on duty cycle (0-100%)*/
    uint32_t matchValue = CTIMER_CLK_FREQ / frequency;
    
    /*Configure PWM for Match 2 (buzzer output)*/
    ctimer_match_config_t matchConfig;
    matchConfig.enableCounterReset = true;
    matchConfig.enableCounterStop = false;
    matchConfig.matchValue = matchValue;
    matchConfig.outControl = kCTIMER_Output_Toggle;
    matchConfig.outPinInitState = true;
    matchConfig.enableInterrupt = false;
    
    CTIMER_SetupMatch(CTIMER, CTIMER_MAT_OUT_BUZZER, &matchConfig);
}

/**
 * @brief Disable buzzer output
 */
void DisableBuzzer() {
    // Turn off PWM output
    ctimer_match_config_t matchConfig;
    matchConfig.enableCounterReset = false;
    matchConfig.enableCounterStop = false;
    matchConfig.matchValue = 0;
    matchConfig.outControl = kCTIMER_Output_NoAction;
    matchConfig.outPinInitState = false;
    matchConfig.enableInterrupt = false;
    
    CTIMER_SetupMatch(CTIMER, CTIMER_MAT_OUT_BUZZER, &matchConfig);
}
/******************************************************************************/
