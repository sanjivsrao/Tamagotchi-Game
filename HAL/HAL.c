/*
 * HAL.c
 *
 *  Created on: Apr 25, 2023
 *      Author: sanji_000
 */

#include <HAL/HAL.h>

void InitGraphics(Graphics_Context *g_sContext_p);

HAL HAL_construct()
{
    // The API object which will be returned at the end of construction
    HAL hal;

    // Construct the joystick
    hal.joystick = Joystick_construct();

    // Once we have finished building the API, return the completed struct.
    return hal;
}

void HAL_refresh(HAL* hal)
{
    Joystick_refresh(&hal->joystick);
}

void initGraphics(Graphics_Context *g_sContext_p) {
    // initializing the display
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // setting up the graphics
    Graphics_initContext(g_sContext_p, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setFont(g_sContext_p, &g_sFontFixed6x8);

    // set colors
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);

    // apply changes by clearing background
    Graphics_clearDisplay(g_sContext_p);
}
