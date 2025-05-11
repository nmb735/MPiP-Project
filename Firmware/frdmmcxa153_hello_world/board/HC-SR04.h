#ifndef _HC_SR04_H_
#define _HC_SR04_H_

#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_ctimer.h"
#include "alarm.h"

/* Define PIN names - replace with actual pins when you configure them */
#define HCSR04_TRIG_GPIO      BOARD_INITPINS_HCSR04_TRIG_GPIO
#define HCSR04_TRIG_PIN       BOARD_INITPINS_HCSR04_TRIG_GPIO_PIN
#define HCSR04_TRIG_PIN_MASK  BOARD_INITPINS_HCSR04_TRIG_GPIO_PIN_MASK

#define HCSR04_ECHO_GPIO      BOARD_INITPINS_HCSR04_ECHO_GPIO
#define HCSR04_ECHO_PIN       BOARD_INITPINS_HCSR04_ECHO_GPIO_PIN
#define HCSR04_ECHO_PIN_MASK  BOARD_INITPINS_HCSR04_ECHO_GPIO_PIN_MASK

/* Constants for distance calculation */
#define SOUND_SPEED          343.0f    /* Speed of sound in m/s at 20°C */
#define MAX_DISTANCE_CM      400       /* Maximum distance in cm */
#define TRIGGER_PULSE_US     10        /* Trigger pulse width in microseconds */

/* Presence detection thresholds (in cm) */
#define PRESENCE_NONE        400.0f       /* No presence detected (beyond 4m) */
#define PRESENCE_LOW         300.0f       /* Low presence detected (3-4m) */
#define PRESENCE_MEDIUM      150.0f       /* Medium presence detected (1.5-3m) */
#define PRESENCE_HIGH        50.0f        /* High presence detected (<0.5m) */

#define HCSR04_TIMER CTIMER2

/* Function prototypes */
void InitHCSR04(void);
float TriggerMeasurement(void);
alarmLevel GetPresenceLevel(float distance);

#endif /* _HC_SR04_H_ */
