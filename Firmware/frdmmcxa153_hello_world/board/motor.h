/*
 * motor.h
 *
 *  Created on: May 10, 2025
 *      Author: NMB
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "alarm.h"

#include "fsl_clock.h"
#include "fsl_reset.h"
#include <stdbool.h>

void init_motor();
void rotate_motor();



#endif /* MOTOR_H_ */
