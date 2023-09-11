
#ifndef HAL_JOYSTICK_H_
#define HAL_JOYSTICK_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Joystick Threshold values
#define LEFT_THRESHOLD  1500
#define RIGHT_THRESHOLD 14000
#define UPPER_THRESHOLD 14000
#define LOWER_THRESHOLD 2000

volatile static bool JoystickX_modified;
volatile static bool JoystickY_modified;

struct _Joystick
{
    uint_fast16_t x;
    uint_fast16_t y;

    bool isTappedToLeft;
    bool isTappedToRight;
    bool isTappedUp;
    bool isTappedDown;
};
typedef struct _Joystick Joystick;

enum _JSDebounceStateLeft {LEFT, NOT_LEFT};
typedef enum _JSDebounceStateLeft JSDebounceStateLeft;

enum _JSDebounceStateRight {RIGHT, NOT_RIGHT};
typedef enum _JSDebounceStateRight JSDebounceStateRight;

enum _JSDebounceStateUp {UP, NOT_UP};
typedef enum _JSDebounceStateUp JSDebounceStateUp;

enum _JSDebounceStateDown {DOWN, NOT_DOWN};
typedef enum _JSDebounceStateDown JSDebounceStateDown;


/** Constructs a new Joystick object, given a valid port and pin. */
Joystick Joystick_construct();

void switch_Left(JSDebounceStateLeft state, Joystick* Joystick_p);
void switch_Right(JSDebounceStateRight state, Joystick* Joystick_p);
void switch_Up(JSDebounceStateUp state, Joystick* Joystick_p);
void switch_Down(JSDebounceStateDown state, Joystick* Joystick_p);


/** Given a Joystick, determines if the joystick is currently pushed to left */
bool Joystick_isPressedToLeft(Joystick* Joystick);

/** Given a Joystick, determines if the joystick is currently pushed to right */
bool Joystick_isPressedToRight(Joystick* Joystick);

/** Given a Joystick, determines if the joystick is currently pushed up */
bool Joystick_isPressedUp(Joystick* Joystick);

/** Given a Joystick, determines if the joystick is currently pushed down */
bool Joystick_isPressedDown(Joystick* Joystick);

/** Given a Joystick, determines if it was "tapped" left */
bool Joystick_isTappedToLeft(Joystick* Joystick);

/** Given a Joystick, determines if it was "tapped" right */
bool Joystick_isTappedToRight(Joystick* Joystick);

bool Joystick_isTappedUp(Joystick* Joystick_p);
bool Joystick_isTappedDown(Joystick* Joystick_p);

/** Refreshes this Joystick so the Joystick FSM now has new outputs to interpret */
void Joystick_refresh(Joystick* Joystick);


#endif /* HAL_JOYSTICK_H_ */

