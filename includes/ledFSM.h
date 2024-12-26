//
// Created by Will Snyder on 12/25/24.
//

#pragma once

#include <Arduino.h>

enum LEDStates{LED_INIT, OFF, ON};

int ledTick(int);
int ledTransition(int);
void ledAction(int);