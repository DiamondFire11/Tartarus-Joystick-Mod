//
// Created by Will Snyder on 12/25/24.
//

#pragma once

#include "TartarusJoystick.h"   // Joystick core code
#include <Arduino.h>

enum JoystickStates{JOYSTICK_INIT, CALIBRATE, READ, PUBLISH_JOYSTICK, PUBLISH_KEYS};

int joystickTick(int);
int joystickTransition(int);
void joystickAction(int);