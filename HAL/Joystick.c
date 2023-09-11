/*
 * Joystick.c
 *
 *  Created on: Apr 25, 2023
 *      Author: sanji_000
 */


#include <HAL/Joystick.h>
#include <HAL/Button.h>


void ADC14_IRQHandler() {
    if (ADC14_getEnabledInterruptStatus() && ADC_INT0) {
        JoystickX_modified = true;
    }
    ADC14_clearInterruptFlag(ADC_INT0);

    if (ADC14_getEnabledInterruptStatus() && ADC_INT1) {
        JoystickY_modified = true;
    }
    ADC14_clearInterruptFlag(ADC_INT1);
}

// Initializing the ADC which resides on SoC
void initADC() {
    ADC14_enableModule();

    ADC14_initModule(ADC_CLOCKSOURCE_SYSOSC,
                     ADC_PREDIVIDER_1,
                     ADC_DIVIDER_1,
                      0
                     );

    // This configures the ADC to store output results
    // in ADC_MEM0 for Joystick X.
    // Todo: if we want to add Joystick Y, then, we have to use more memory locations
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

    ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_192, ADC_PULSE_WIDTH_192);

    // This configures the ADC in manual conversion mode
    // Software will start each conversion.
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
}


void startADC() {
   // Starts the ADC with the first conversion
   // in repeat-mode, subsequent conversions run automatically
   ADC14_enableConversion();
   ADC14_toggleConversionTrigger();
}


// Interfacing the Joystick with ADC (making the proper connections in software)
void initJoystick() {

    // This configures ADC_MEM0 to store the result from
    // input channel A15 (Joystick X), in non-differential input mode
    // (non-differential means: only a single input pin)
    // The reference for Vref- and Vref+ are VSS and VCC respectively
    ADC14_configureConversionMemory(ADC_MEM0,
                                  ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                  ADC_INPUT_A15,                 // Joystick X
                                  ADC_NONDIFFERENTIAL_INPUTS);

    // This selects the GPIO as analog input
    // A15 is multiplexed on GPIO port P6 pin PIN0
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0,
                                               GPIO_TERTIARY_MODULE_FUNCTION);


    // This configures ADC_MEM0 to store the result from
    // input channel A15 (Joystick X), in non-differential input mode
    // (non-differential means: only a single input pin)
    // The reference for Vref- and Vref+ are VSS and VCC respectively
    ADC14_configureConversionMemory(ADC_MEM1,
                                  ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                  ADC_INPUT_A9,                 // Joystick X
                                  ADC_NONDIFFERENTIAL_INPUTS);

    // This selects the GPIO as analog input
    // A15 is multiplexed on GPIO port P6 pin PIN0
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN4,
                                               GPIO_TERTIARY_MODULE_FUNCTION);

    ADC14_clearInterruptFlag(ADC_INT0);
    ADC14_enableInterrupt(ADC_INT0);

    ADC14_clearInterruptFlag(ADC_INT1);
    ADC14_enableInterrupt(ADC_INT1);

    Interrupt_enableInterrupt(INT_ADC14);
}


Joystick Joystick_construct()
{
    // The Joystick object which will be returned at the end of construction
    Joystick Joystick;

    initADC();
    initJoystick();
    startADC();

//    // Initialize all buffered outputs of the Joystick
//    Joystick.pushState = RELEASED;
//    Joystick.isTapped = false;
//
    // Return the constructed Joystick object to the user
    return Joystick;
}

void Joystick_refresh(Joystick* Joystick_p){
    if (JoystickX_modified) {
        Joystick_p->x = ADC14_getResult(ADC_MEM0);
        JoystickX_modified = false;
    }

    if (JoystickY_modified) {
        Joystick_p->y = ADC14_getResult(ADC_MEM1);
        JoystickY_modified = false;
    }

    static JSDebounceStateLeft L_state = NOT_LEFT;
    Joystick_p->isTappedToLeft = false;

    static JSDebounceStateRight R_state = NOT_RIGHT;
    Joystick_p->isTappedToRight = false;

    static JSDebounceStateUp U_state = NOT_UP;
    Joystick_p->isTappedUp = false;

    static JSDebounceStateDown D_state = NOT_DOWN;
    Joystick_p->isTappedDown = false;

    switch (L_state){
    case NOT_LEFT:
        if (Joystick_p->x < LEFT_THRESHOLD){
            L_state = LEFT;
            Joystick_p->isTappedToLeft = true;
        }
        break;
    case LEFT:
        if (Joystick_p->x > LEFT_THRESHOLD){
            L_state = NOT_LEFT;
        }
        break;
    }
    switch (R_state){
    case NOT_RIGHT:
        if (Joystick_p->x > RIGHT_THRESHOLD){
            R_state = RIGHT;
            Joystick_p->isTappedToRight = true;
        }
        break;
    case RIGHT:
        if (Joystick_p->x < RIGHT_THRESHOLD){
            R_state = NOT_RIGHT;
        }
        break;
    }
    switch (U_state){
    case NOT_UP:
        if (Joystick_p->y > UPPER_THRESHOLD){
            U_state = UP;
            Joystick_p->isTappedUp = true;
        }
        break;
    case UP:
        if (Joystick_p->y < UPPER_THRESHOLD){
            U_state = NOT_UP;
        }
        break;
    }
    switch (D_state){
    case NOT_DOWN:
        if (Joystick_p->y < LOWER_THRESHOLD){
            D_state = DOWN;
            Joystick_p->isTappedDown = true;
        }
        break;
    case RIGHT:
        if (Joystick_p->y > LOWER_THRESHOLD){
            D_state = NOT_DOWN;
        }
        break;
    }
}


bool Joystick_isPressedToLeft(Joystick* Joystick_p){
    if (Joystick_p->x < LEFT_THRESHOLD)
        return true;
    else
        return false;
}

bool Joystick_isPressedToRight(Joystick* Joystick_p){
    if (Joystick_p->x > RIGHT_THRESHOLD)
        return true;
    else
        return false;
}

bool Joystick_isPressedUp(Joystick* Joystick_p){
    if (Joystick_p->x > UPPER_THRESHOLD)
        return true;
    else
        return false;
}

bool Joystick_isPressedDown(Joystick* Joystick_p){
    if (Joystick_p->x < LOWER_THRESHOLD)
        return true;
    else
        return false;
}

bool Joystick_isTappedToLeft(Joystick* Joystick_p){
    return (Joystick_p->isTappedToLeft);
}


bool Joystick_isTappedToRight(Joystick* Joystick_p){
    return (Joystick_p->isTappedToRight);
}


bool Joystick_isTappedUp(Joystick* Joystick_p){
    return (Joystick_p->isTappedUp);
}


bool Joystick_isTappedDown(Joystick* Joystick_p){
    return (Joystick_p->isTappedDown);
}


