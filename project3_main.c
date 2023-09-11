// Starter code for Project 3. Good luck!

/* TI includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <LcdDriver/Crystalfontz128x128_ST7735.h>

/* Standard includes */
#include <stdio.h>

/* HAL includes */
#include "HAL/LED.h"
#include "HAL/Timer.h"
#include "HAL/Button.h"
#include "HAL/Joystick.h"
#include "HAL/HAL.h"
#include "proj3_head.h"

int main(void)
{

    initialize();

    /* Graphics context. Works in the same as it did in the previous projects.
       You may choose to make a HAL struct and store the Graphics context in there instead.
       You may also choose to use the GFX struct from Guess the Color. */
    Graphics_Context g_sContext;
    initGraphics(&g_sContext);

    HAL hal = HAL_construct();
    App_proj3 app = App_proj3_construct(&hal, &g_sContext);

    while (1) {
        sleep();
        main_loop(&app, &g_sContext, &hal);
        HAL_refresh(&hal); // check the inputs
        buttons_t buttons = updateButtons();
    }
}

void sleep() {
    // The Launchpad Green LED is used to signify the processor is in low-power mode.
    // From the human perspective, it should seem the processor is always asleep except for fractions of second here and there.

    TurnOn_LLG();
    // Enters the Low Power Mode 0 - the processor is asleep and only responds to interrupts
    PCM_gotoLPM0();
    TurnOff_LLG();

}

App_proj3 App_proj3_construct(HAL* hal_p, Graphics_Context *g_sContext_p) {
    // The App_proj2 object to initialize
    App_proj3 app;

    // Initialization of FSM variables
    app.state = TITLE;
    app.happiness = INIT_HAPPINESS;
    app.energy = INIT_ENERGY;
    app.age = 0;
    app.position = INIT_POSITION;
    app.exhaustion = 0;
    app.level = 1;

    Timer32_initModule (TIMER32_1_BASE, TIMER32_PRESCALER_256, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_1_BASE, THREE_SECOND);
    Timer32_startTimer(TIMER32_1_BASE, true);
    App_proj3_showTitleScreen(g_sContext_p);


    // Return the completed App_proj2 struct to the user
    return app;
}

void main_loop(App_proj3 *app_p, Graphics_Context *g_sContext_p, HAL* hal_p) {
    buttons_t buttons = updateButtons();

    if (buttons.JSBtapped)
        Toggle_BLG();

    switch (app_p->state) {
    case TITLE:
        App_proj3_handleTitleScreen(app_p, g_sContext_p);
        break;

    case INSTRUCTIONS:
        if (buttons.BB1tapped) {
            App_proj3_handleInstructionsScreen(app_p, g_sContext_p);
        }
        break;

    case GAME:{
        if (buttons.BB1tapped) {
            app_p->energy++;
            App_proj3_updateEnergy(app_p, g_sContext_p);
        }
        int limit_l = app_p->position-app_p->level*8;
        int limit_r = app_p->position+app_p->level*8;
        if (app_p->energy != 0){
            if (Joystick_isTappedToLeft(&hal_p->joystick) && (limit_l-8)>=LEFT_BOUND){
                App_proj3_movePetLeft(app_p, g_sContext_p);
            }
            else if (Joystick_isTappedToRight(&hal_p->joystick) && (limit_r+8)<=RIGHT_BOUND) {
                App_proj3_movePetRight(app_p, g_sContext_p);
            }
        }
        App_proj3_handleGameScreen(app_p, g_sContext_p);
        if (Timer32_getValue(TIMER32_1_BASE) == 0) {
            Timer32_setCount(TIMER32_1_BASE, THREE_SECOND);
            Timer32_startTimer(TIMER32_1_BASE, true);
            App_proj3_updateAge(app_p, g_sContext_p);
            app_p->energy--;
            App_proj3_updateEnergy(app_p, g_sContext_p);
            app_p->happiness--;
            App_proj3_updateHappiness(app_p, g_sContext_p);
        }
        if (app_p->energy == 0 && app_p->happiness == 0){
            App_proj3_showEndScreen(app_p,g_sContext_p);
            app_p->state = END;
            break;
        }
        break;}

    case END:
        if (buttons.BB1tapped) {
            App_proj3_handleEndScreen(app_p,g_sContext_p);
        }
        break;

    default:
        break;
    }

}

// Initialization part is always device dependent and therefore does not change much with HAL
void initialize()
{

    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);
    InitSystemTiming();

    initLEDs();
    initButtons();

}




void App_proj3_handleTitleScreen(App_proj3* app_p, Graphics_Context *g_sContext_p) {
    if (Timer32_getValue(TIMER32_1_BASE) == 0) {
        Graphics_clearDisplay(g_sContext_p);
        App_proj3_showInstructionsScreen(g_sContext_p);
        app_p->state = INSTRUCTIONS;
    }
}

void App_proj3_handleEndScreen(App_proj3* app_p, Graphics_Context *g_sContext_p) {
    Graphics_clearDisplay(g_sContext_p);
    App_proj3_showInstructionsScreen(g_sContext_p);
    app_p->age = 0;
    app_p->energy = INIT_ENERGY;
    app_p->happiness = INIT_HAPPINESS;
    app_p->position = INIT_POSITION;
    app_p->exhaustion = 0;
    app_p->state = INSTRUCTIONS;
}

void App_proj3_handleInstructionsScreen(App_proj3* app_p, Graphics_Context *g_sContext_p) {
    Graphics_clearDisplay(g_sContext_p);
    App_proj3_showGameScreen(app_p, g_sContext_p);
    Timer32_setCount(TIMER32_1_BASE, THREE_SECOND);
    Timer32_startTimer(TIMER32_1_BASE, true);
    app_p->state = GAME;
}

void App_proj3_handleGameScreen(App_proj3* app_p, Graphics_Context *g_sContext_p) {

    if (app_p->level ==1 && app_p->age >= 3 && app_p->happiness >= 4 && app_p->energy >= 3){
        app_p->level++;
    }
    if (app_p->level ==2 && app_p->age >= 7 && app_p->happiness >= 2 && app_p->energy >= 2){
        app_p->level++;
    }

}

void App_proj3_showTitleScreen(Graphics_Context *g_sContext_p) {
    Graphics_drawString(g_sContext_p, "ECE 2564", 8, 40, 10, true);
    Graphics_drawString(g_sContext_p, "Project 3", 9, 36, 20, true);
    Graphics_drawString(g_sContext_p, "Sanjiv Rao", 10, 30, 30, true);
    Graphics_drawString(g_sContext_p, "Pet Sim 4000", 12, 24, 40, true);
}

void App_proj3_showInstructionsScreen(Graphics_Context *g_sContext_p) {
    Graphics_drawString(g_sContext_p, "Welcome to", 21, 32, 10, true);
    Graphics_drawString(g_sContext_p, "Pet Sim 4000!", 21, 30, 20, true);
    Graphics_drawString(g_sContext_p, "You have adopted", 21, 1, 39, true);
    Graphics_drawString(g_sContext_p, "a new pet! Play", 21, 1, 49, true);
    Graphics_drawString(g_sContext_p, "with it by moving ", 21, 1, 59, true);
    Graphics_drawString(g_sContext_p, "w/ JS. As you do,", 21, 1, 69, true);
    Graphics_drawString(g_sContext_p, "your pet will grow,", 21, 1, 79, true);
    Graphics_drawString(g_sContext_p, "but to grow it needs", 21, 1, 89, true);
    Graphics_drawString(g_sContext_p, "energy! Feed your Pet", 21, 1, 99, true);
    Graphics_drawString(g_sContext_p, "with BB1 and play to", 21, 1, 109, true);
    Graphics_drawString(g_sContext_p, "keep it happy!", 21, 1, 119, true);

}

void App_proj3_showGameScreen(App_proj3* app_p, Graphics_Context *g_sContext_p){
    Graphics_Rectangle playpen = {LEFT_BOUND, 60, RIGHT_BOUND, 120};
    Graphics_drawRectangle(g_sContext_p, &playpen);
    Graphics_drawString(g_sContext_p, "Age:", 21, LEFT_BOUND, 10, true);
    Graphics_drawString(g_sContext_p, "Energy:", 21, LEFT_BOUND, 30, true);
    Graphics_drawString(g_sContext_p, "Happiness:", 21, LEFT_BOUND, 50, true);
    App_proj3_drawPet(g_sContext_p, app_p->position, app_p->level);
    char print_age[] = "";
    char x = snprintf(print_age, 4, "%d",app_p->age);
    Graphics_drawString(g_sContext_p, print_age, 3, 75, 10, true);
    Graphics_drawString(g_sContext_p, "*****", app_p->energy, 75, 30, true);
    Graphics_drawString(g_sContext_p, "*****", app_p->happiness, 75, 50, true);
}

void App_proj3_showEndScreen(App_proj3* app_p, Graphics_Context *g_sContext_p) {
    Graphics_clearDisplay(g_sContext_p);
    Graphics_drawString(g_sContext_p, "Your pet ran away", 21, LEFT_BOUND, 10, true);
    Graphics_drawString(g_sContext_p, "Age:", 21, LEFT_BOUND, 20, true);
    char print_age[] = "";
    char x = snprintf(print_age, 4, "%d",app_p->age);
    Graphics_drawString(g_sContext_p, print_age, 3, 35, 20, true);
    Graphics_drawString(g_sContext_p, "Press BB1 to restart", 21, LEFT_BOUND, 30, true);

}

void App_proj3_drawPet(Graphics_Context *g_sContext_p, int position, int level) {
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_ROSY_BROWN);
    Graphics_fillCircle(g_sContext_p, position, 90, level*8);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
}

void App_proj3_erasePet(Graphics_Context *g_sContext_p, int position, int level) {
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_fillCircle(g_sContext_p, position, 90, level*8);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
}


void App_proj3_movePetLeft(App_proj3* app_p, Graphics_Context *g_sContext_p) {
    App_proj3_erasePet(g_sContext_p, app_p->position, app_p->level);
    app_p->position -= 8;
    App_proj3_drawPet(g_sContext_p, app_p->position, app_p->level);
    if (app_p->exhaustion == 0) {
        app_p->exhaustion++;
    }
    else {
        app_p->exhaustion = 0;
        app_p->energy--;
        app_p->happiness++;
        App_proj3_updateEnergy(app_p, g_sContext_p);
        App_proj3_updateHappiness(app_p, g_sContext_p);
    }
}

void App_proj3_movePetRight(App_proj3* app_p, Graphics_Context *g_sContext_p) {
    App_proj3_erasePet(g_sContext_p, app_p->position, app_p->level);
    app_p->position += 8;
    App_proj3_drawPet(g_sContext_p, app_p->position, app_p->level);
    if (app_p->exhaustion == 0) {
        app_p->exhaustion++;
    }
    else {
        app_p->exhaustion = 0;
        app_p->energy--;
        app_p->happiness++;
        App_proj3_updateEnergy(app_p, g_sContext_p);
        App_proj3_updateHappiness(app_p, g_sContext_p);
    }
}

void App_proj3_updateEnergy(App_proj3* app_p, Graphics_Context *g_sContext_p) {

    if (app_p->energy < 0) {
        app_p->energy = 0 ;
    }
    else if (app_p->energy > 5) {
        app_p->energy = 5;
    }
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(g_sContext_p, "*****", 5, 75, 30, true);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(g_sContext_p, "*****", app_p->energy, 75, 30, true);
}

void App_proj3_updateAge(App_proj3* app_p, Graphics_Context *g_sContext_p) {
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    char print_age[] = "";
    char x = snprintf(print_age, 3, "%d",app_p->age);
    Graphics_drawString(g_sContext_p, print_age, 2, 75, 10, true);
    app_p->age++;
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    char y = snprintf(print_age, 3, "%d",app_p->age);
    Graphics_drawString(g_sContext_p, print_age, 2, 75, 10, true);
}

void App_proj3_updateHappiness(App_proj3* app_p, Graphics_Context *g_sContext_p) {
    if (app_p->happiness < 0) {
        app_p->happiness = 0 ;
    }
    else if (app_p->happiness > 5) {
        app_p->happiness = 5;
    }
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(g_sContext_p, "*****", 5, 75, 50, true);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(g_sContext_p, "*****", app_p->happiness, 75, 50, true);
}





