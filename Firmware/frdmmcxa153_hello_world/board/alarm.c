/*
 * alarm.c
 *
 *  Created on: May 10, 2025
 *      Author: NMB
 */

#include "alarm.h"

void InitAlarm(uint32_t frequencyHz){

	// Set up LED GPIOs
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput, 0,  // Start with logic low
    };
    GPIO_PinInit(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN, &led_config);
    GPIO_PinInit(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN, &led_config);
    GPIO_PinInit(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN, &led_config);

    // Turn on green LED (control LED)
    GPIO_PortSet(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);

	// Initialize the buzzer PWM using CTIMER
    ctimer_config_t config;
    //ctimer_match_config_t matchReset;
    ctimer_match_config_t matchConfig;

    CLOCK_EnableClock(kCLOCK_GateGPIO3);
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER1, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER1);

    CTIMER_GetDefaultConfig(&config);
	//RESET_PeripheralReset(kCTIMER1_RST_SHIFT_RSTn);
    CTIMER_Init(CTIMER, &config);

    // Configure PWM mode for buzzer
    uint32_t pwmFreq = frequencyHz; // Frequency in Hz
    uint32_t matchValue = CTIMER_CLK_FREQ / pwmFreq;

    // Initialize PWM
    matchConfig.enableCounterReset = true;
    matchConfig.enableCounterStop = false;
    matchConfig.matchValue = matchValue;
    matchConfig.outControl = kCTIMER_Output_NoAction;
    matchConfig.outPinInitState = false;
    matchConfig.enableInterrupt = false;
    
    // Match register 0 sets the PWM period
    CTIMER_SetupMatch(CTIMER, CTIMER_MAT_PWM_PERIOD_CHANNEL, &matchConfig);
    
    // Start timer
    CTIMER_StartTimer(CTIMER);
    
    // Initially, buzzer is off
    ClearAlarmLevel();
}


void UpdateAlarm(alarmLevel alarm){
	switch(alarm){
		case ALARM_LEVEL_NONE:
			ClearAlarmLevel();
			break;
		case ALARM_LEVEL_LOW:
			SetAlarmLevelLow();
			break;
		case ALARM_LEVEL_MEDIUM:
			SetAlarmLevelMedium();
			break;
		case ALARM_LEVEL_HIGH:
			SetAlarmLevelHigh();
			break;
		default:
			ClearAlarmLevel();
	}
}

void SetAlarmLevelLow(){
    GPIO_PortClear(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);
    GPIO_PortSet(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN_MASK);
    DisableBuzzer();
}

void SetAlarmLevelMedium(){
    GPIO_PortClear(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);
    GPIO_PortSet(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN_MASK);
	SetBuzzerIntensity(1);
}

void SetAlarmLevelHigh(){
    GPIO_PortSet(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN_MASK);
    GPIO_PortSet(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);
	SetBuzzerIntensity(90);
}

void ClearAlarmLevel(){
    GPIO_PortClear(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_GPIO_PIN_MASK);
    GPIO_PortClear(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);
    GPIO_PortSet(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_GPIO_PIN_MASK);
	DisableBuzzer();
}

void SetBuzzerIntensity(uint8_t dutyCycle) {
    // Get PWM period from Match 0
    uint32_t period = CTIMER->MR[CTIMER_MAT_PWM_PERIOD_CHANNEL];
    
    // Calculate match value based on duty cycle (0-100%)
    uint32_t matchValue = period - ((period * dutyCycle) / 100);
    
    // Configure PWM for Match 2 (buzzer output)
    ctimer_match_config_t matchConfig;
    matchConfig.enableCounterReset = false;
    matchConfig.enableCounterStop = false;
    matchConfig.matchValue = matchValue;
    matchConfig.outControl = kCTIMER_Output_Toggle;
    matchConfig.outPinInitState = true;
    matchConfig.enableInterrupt = false;
    
    CTIMER_SetupMatch(CTIMER, CTIMER_MAT_OUT_BUZZER, &matchConfig);
}

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

