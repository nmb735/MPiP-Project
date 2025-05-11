/*******************************************************************************
 * Alarm System Library
 *
 * File: alarm.h
 * Description: Header file for alarm system interface. Provides functionality
 *              for controlling visual (LEDs) and audible (buzzer) alerts with
 *              variable intensity based on threat level.
 *
 * Created on: May 10, 2025
 * Author: Nedal M. Benelmekki
 ******************************************************************************/

#ifndef ALARM_H_
#define ALARM_H_

/******************************************************************************/
/* Includes                                                                   */
/******************************************************************************/
#include "fsl_device_registers.h" /* Device Headers                           */
#include "fsl_debug_console.h"    /* Debug console                            */
#include "pin_mux.h"              /* Pins                                     */
#include "board.h"                /* Development Board                        */
#include "fsl_ctimer.h"           /* CTIMER driver                            */
#include "fsl_clock.h"            /* Clock                                    */
#include "fsl_reset.h"            /* Reset                                    */
#include <stdbool.h>              /* Standard boolean type                    */
/******************************************************************************/


/******************************************************************************/
/* Definitions                                                                */
/******************************************************************************/
/* CTIMER configurations for buzzer PWM generation                            */
#define CTIMER                  CTIMER1   /* Timer used for buzzer control    */
#define CTIMER_MAT_RESET        kCTIMER_Match_0 /* Timer reset match channel  */
#define CTIMER_MAT_OUT_BUZZER   kCTIMER_Match_3 /* Buzzer output channel      */
#define CTIMER_CLK_FREQ         CLOCK_GetCTimerClkFreq(1U) /* Timer frequency */
/* PWM channel definition                                                     */
#ifndef CTIMER_MAT_PWM_PERIOD_CHANNEL											
#define CTIMER_MAT_PWM_PERIOD_CHANNEL kCTIMER_Match_0 /* Default period channel */
#endif
/******************************************************************************/


/******************************************************************************/
/* Type Definitions                                                           */
/******************************************************************************/
/**
 * @brief Alarm level enumeration defining different threat levels
 */
typedef enum alarmLevel {
    ALARM_LEVEL_NONE,     /* No active alarm (system monitoring)              */
    ALARM_LEVEL_LOW,      /* Low threat level (green LED)                     */
    ALARM_LEVEL_MEDIUM,   /* Medium threat level (yellow LED + slow buzzer)   */
    ALARM_LEVEL_HIGH      /* High threat level (red LED + fast buzzer)        */
} alarmLevel;
/******************************************************************************/


/******************************************************************************/
/* Function Prototypes                                                        */
/******************************************************************************/
/**
 * @brief Initialize the alarm system (LEDs and buzzer)
 * @param frequencyHz Base frequency for buzzer in Hertz
 */
void InitAlarm(uint32_t frequencyHz);

/**
 * @brief Update alarm indicators based on threat level
 * @param level The alarm level to set (from alarmLevel enum)
 */
void UpdateAlarm(alarmLevel level);

/**
 * @brief Set alarm to low level (green LED only)
 */
void SetAlarmLevelLow(void);

/**
 * @brief Set alarm to medium level (yellow LED and slow buzzer)
 */
void SetAlarmLevelMedium(void);

/**
 * @brief Set alarm to high level (red LED and fast buzzer)
 */
void SetAlarmLevelHigh(void);

/**
 * @brief Clear all alarms (restore normal state)
 */
void ClearAlarmLevel(void);

/**
 * @brief Set buzzer intensity via duty cycle
 * @param dutyCycle PWM duty cycle (0-100%)
 */
void SetBuzzerIntensity(uint8_t dutyCycle);

/**
 * @brief Disable buzzer output
 */
void DisableBuzzer(void);
/******************************************************************************/

#endif /* ALARM_H_ */