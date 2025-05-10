/*
 * alarm.h
 *
 *  Created on: May 10, 2025
 *      Author: NMB
 */

#ifndef ALARM_H_
#define ALARM_H_

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_ctimer.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include <stdbool.h>

#define CTIMER CTIMER1
#define CTIMER_MAT_RESET kCTIMER_Match_0
#define CTIMER_MAT_OUT_BUZZER kCTIMER_Match_3
#define CTIMER_CLK_FREQ CLOCK_GetCTimerClkFreq(1U)

#ifndef CTIMER_MAT_PWM_PERIOD_CHANNEL
#define CTIMER_MAT_PWM_PERIOD_CHANNEL kCTIMER_Match_0
#endif


typedef enum alarmLevel {
	ALARM_LEVEL_NONE,
	ALARM_LEVEL_LOW,
	ALARM_LEVEL_MEDIUM,
	ALARM_LEVEL_HIGH
}alarmLevel;

void InitAlarm(uint32_t frequencyHz);

void UpdateAlarm(alarmLevel);

void SetAlarmLevelLow();

void SetAlarmLevelMedium();

void SetAlarmLevelHigh();

void ClearAlarmLevel();

void SetBuzzerIntensity(uint8_t dutyCycle);

void DisableBuzzer();



#endif /* ALARM_H_ */
