//
// Created by Will Snyder on 12/25/24.
//

#include "includes/joystickFSM.h"

TartarusJoystick joystick;
uint32_t calibrationTime;

bool isCalibrated = false;
bool isKeyboard = true;


int joystickTick(int state) {
    state = joystickTransition(state);
    joystickAction(state);
    return state;
}

int joystickTransition(int state) {
    switch (state) {
        case JoystickStates::JOYSTICK_INIT:
            calibrationTime = millis();
            state = JoystickStates::CALIBRATE;
            break;

        case JoystickStates::CALIBRATE:
            if((millis() - calibrationTime) >= 5000 || isCalibrated) {
                state = JoystickStates::READ;
            } else {
                state = JoystickStates::CALIBRATE;
            }
            break;

        case JoystickStates::READ:
            if(isKeyboard) {
                state = JoystickStates::PUBLISH_KEYS;
            } else {
                state = JoystickStates::PUBLISH_JOYSTICK;
            }
            break;

        case JoystickStates::PUBLISH_KEYS:
        case JoystickStates::PUBLISH_JOYSTICK:
            state = JoystickStates::READ;
            break;

        default:
            Serial.println("Error: Encountered unknown state, resetting SM.");
            state = JoystickStates::JOYSTICK_INIT;
    }

    return state;
}

void joystickAction(int state) {
    switch (state) {
        case JoystickStates::JOYSTICK_INIT:
            isKeyboard = true;
            isCalibrated = false;
            break;

        case JoystickStates::CALIBRATE:
            // If button pressed and Joystick is moved to -X begin calibration
            if(analogRead(A4) <= 300 && digitalRead(JOYSTICK_BUTTON)) {
                joystick.calibrate();
                isCalibrated = true;
            }

            // If joystick is moved to +X axis -> Set to joystick mode
            if(analogRead(A4) >= 700) {
                isKeyboard = false;
            }
            break;

        case JoystickStates::READ:
            joystick.readJoystickData();
            joystick.printToSerial();
            break;

        case JoystickStates::PUBLISH_JOYSTICK:
            // Implement with joystick code
            break;

        case JoystickStates::PUBLISH_KEYS:
            joystick.publishKeystrokes();
            break;

        default:
            isKeyboard = true;
            isCalibrated = false;
    }
}