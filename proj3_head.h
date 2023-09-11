/*
 * proj3_head.h
 *
 *  Created on: Apr 25, 2023
 *      Author: Sanjiv Rao
 */

#ifndef proj3_head_H_
#define proj3_head_H_

#include <HAL/Timer.h>
#include <HAL/Button.h>
#include <HAL/Joystick.h>
#include <HAL/LED.h>

#define THREE_SECOND 562500
#define LEFT_BOUND 10
#define RIGHT_BOUND 118
#define INIT_ENERGY 5
#define INIT_HAPPINESS 3
#define INIT_POSITION 65

enum _ProgramState {
    TITLE, INSTRUCTIONS, GAME, END
};
typedef enum _ProgramState ProgramState;

struct _App_proj3 {
    ProgramState state;  // Determines which screen is currently shown
    int energy;
    int happiness;
    int age;
    int position;
    int exhaustion;
    int level;
};
typedef struct _App_proj3 App_proj3;

// Boilerplate constructor function for the Application
App_proj3 App_proj3_construct(HAL* hal_p, Graphics_Context *g_sContext_p);
void initialize();
void initGraphics(Graphics_Context *g_sContext_p);
void main_loop(App_proj3* app_p, Graphics_Context *g_sContext_p, HAL* hal_p);
void sleep();

// Handle callback functions used for each state of the App_proj3_loop()
// function. State transitions are embedded inside of each of these functions.
void App_proj3_handleTitleScreen(App_proj3* app_p, Graphics_Context *g_sContext_p);
void App_proj3_handleInstructionsScreen(App_proj3* app_p, Graphics_Context *g_sContext_p);
void App_proj3_handleGameScreen(App_proj3* app_p, Graphics_Context *g_sContext_p);
void App_proj3_handleEndScreen(App_proj3* app_p, Graphics_Context *g_sContext_p);

// Helper functions which clear the screen and draw the text for each state
void App_proj3_showTitleScreen(Graphics_Context *g_sContext_p);
void App_proj3_showInstructionsScreen(Graphics_Context *g_sContext_p);
void App_proj3_showGameScreen(App_proj3* app_p, Graphics_Context *g_sContext_p);
void App_proj3_showEndScreen(App_proj3* app_p, Graphics_Context *g_sContext_p);


//Pet movement functions
void App_proj3_drawPet(Graphics_Context *g_sContext_p, int position, int level);
void App_proj3_erasePet(Graphics_Context *g_sContext_p, int position, int level);
void App_proj3_movePetLeft(App_proj3* app_p, Graphics_Context *g_sContext_p);
void App_proj3_movePetRight(App_proj3* app_p, Graphics_Context *g_sContext_p);

//Update energy, age, and happiness
void App_proj3_updateEnergy(App_proj3* app_p, Graphics_Context *g_sContext_p);
void App_proj3_updateAge(App_proj3* app_p, Graphics_Context *g_sContext_p);
void App_proj3_updateHappiness(App_proj3* app_p, Graphics_Context *g_sContext_p);

#endif /* proj3_head_H_ */
