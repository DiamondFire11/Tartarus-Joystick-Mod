//
// Created by Will Snyder on 12/26/24.
//

#include "includes/TartarusJoystick.h"

void TartarusJoystick::publishKeystrokes() const {
    double_t angle = atan2(this->_data.position.y, this->_data.position.x) + M_PI; // Range between 0 and 2pi

    // Handle button press
    if(this->_data.button) {
        Keyboard.press(JoystickKeys::KEY_BUTTON);
    } else {
        Keyboard.release(JoystickKeys::KEY_BUTTON);
    }

    // Release direction keys
    Keyboard.release(JoystickKeys::KEY_UP);
    Keyboard.release(JoystickKeys::KEY_DOWN);
    Keyboard.release(JoystickKeys::KEY_LEFT);
    Keyboard.release(JoystickKeys::KEY_RIGHT);

    // If in deadzone release keys and return
    if(this->_data.position.x == 0 && this->_data.position.y == 0) {
        return;
    }

    // 'A' Pressed
    if((5.89 < angle && angle <= 2*M_PI) || (0 <= angle && angle <= 0.39)) {
        Keyboard.press(JoystickKeys::KEY_LEFT);
        return;
    }

    // 'A' 'S' Pressed
    if(0.39 < angle && angle <= 1.17) {
        Keyboard.press(JoystickKeys::KEY_LEFT);
        Keyboard.press(JoystickKeys::KEY_DOWN);
        return;
    }

    // 'S' Pressed
    if(1.17 < angle && angle <= 1.96) {
        Keyboard.press(JoystickKeys::KEY_DOWN);
        return;
    }

    // 'S' 'D' Pressed
    if(1.96 < angle && angle <= 2.74) {
        Keyboard.press(JoystickKeys::KEY_DOWN);
        Keyboard.press(JoystickKeys::KEY_RIGHT);
        return;
    }

    // 'D' Pressed
    if(2.74 < angle && angle <= 3.52) {
        Keyboard.press(JoystickKeys::KEY_RIGHT);
        return;
    }

    // 'D' 'W' Pressed
    if(3.52 < angle && angle <= 4.30) {
        Keyboard.press(JoystickKeys::KEY_RIGHT);
        Keyboard.press(JoystickKeys::KEY_UP);
        return;
    }

    // 'W' Pressed
    if(4.30 < angle && angle <= 5.08) {
        Keyboard.press(JoystickKeys::KEY_UP);
        return;
    }

    // 'W' 'A' Pressed
    if(5.08 < angle && angle <= 5.89) {
        Keyboard.press(JoystickKeys::KEY_UP);
        Keyboard.press(JoystickKeys::KEY_LEFT);
        return;
    }
}
