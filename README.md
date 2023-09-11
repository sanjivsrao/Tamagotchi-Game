# Tamagotchi-Game
The purpose of this project was to develop a minigame to be played on the Texas Instrument’s MSP432P401R Launchpad microcontroller. This was done in order to garner knowledge about interfacing with the microcontroller board through GPIO, timers, ADC with the joystick, and working with the LCD screen.

Project Description:

The application I have generated establishes an interface by which one can play a game
that resembles a Tamagotchi. In this game, you raise a pet which has status values such as
energy, happiness, and age. There are various screens such as a title screen, instructions screen,
end screen, and the game screen. Pressing the BB1 button will increment the energy value and
the happiness will increase every 2 moves. Letting the pet age with appropriate energy and
happiness values allows the pet to level up and age. Letting energy and happiness fall to 0 will
end the game, providing the player an opportunity to play again. All parts of the game are
functional and can be seen by flashing the code.

Code Quality:

In accordance with the requirements defined for this project, the code was designed to be
modular and easily read. All function names are based on their functionality and variables were
declared with understandable names. Similar operations that occurred often were condensed into
functions that could be called when needed. Any variables that were necessary locally were
created with static declarations. Variables that were used by many parts of the application, such
as lives and score, were condensed within another struct. No global variables were utilized aside
from some macros to define commonly used numbers. Hardcoding was avoided as much as
possible, yet some functions such as snprintf would not accept macro-based parameters.
Alongside this, all functions were shortened so they were not 60 lines or longer. All code was
designed to be non-blocking.
