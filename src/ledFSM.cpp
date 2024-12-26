//
// Created by Will on 12/25/24.
//

#include "includes/ledFSM.h"

int ledTick(int state) {
    state = ledTransition(state);
    ledAction(state);
    return state;
}

int ledTransition(int state) {
    switch (state) {
        case LEDStates::LED_INIT:
            state = LEDStates::OFF;
            break;

        case LEDStates::OFF:
            state = LEDStates::ON;
            break;

        case LEDStates::ON:
            state = LEDStates::OFF;
            break;

        default:
            Serial.println("Error: Encountered unknown state, resetting SM.");
            state = LEDStates::LED_INIT;
    }
    return state;
}

void ledAction(int state) {
    switch (state) {
        case LEDStates::LED_INIT:
        case LEDStates::OFF:
            digitalWrite(LED_BUILTIN, LOW);
            break;

        case LEDStates::ON:
            digitalWrite(LED_BUILTIN, HIGH);
            break;

        default:
            digitalWrite(LED_BUILTIN, LOW);
    }
}