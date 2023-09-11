/*
 * HAL.h
 *
 *  Created on: Apr 25, 2023
 *      Author: sanji_000
 */

#ifndef HAL_HAL_H_
#define HAL_HAL_H_

#include <HAL/Button.h>
#include <HAL/LED.h>
#include <HAL/Timer.h>
#include <HAL/Joystick.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

struct _HAL
{
    Joystick joystick;

};
typedef struct _HAL HAL;

// Constructs an HAL object by calling the constructor of each individual member
HAL HAL_construct();

// Refreshes all necessary inputs in the HAL
void HAL_refresh(HAL* api);

#endif /* HAL_HAL_H_ */
