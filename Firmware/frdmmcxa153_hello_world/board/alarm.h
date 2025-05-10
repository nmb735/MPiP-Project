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

#include "fsl_clock.h"
#include "fsl_reset.h"
#include <stdbool.h>

typedef enum alarmLevel {
	ALARM_LEVEL_NONE,
	ALARM_LEVEL_LOW,
	ALARM_LEVEL_MEDIUM,
	ALARM_LEVEL_HIGH
}alarmLevel;

void InitAlarm();

void UpdateAlarm(alarmLevel);

void SetAlarmLevelLow();

void SetAlarmLevelMedium();

void SetAlarmLevelHigh();

void ClearAlarmLevel();



#endif /* ALARM_H_ */
