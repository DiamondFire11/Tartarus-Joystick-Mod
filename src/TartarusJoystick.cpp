//
// Created by Will Snyder on 12/23/24.
//

#include "includes/TartarusJoystick.h"

TartarusJoystick::TartarusJoystick() {
    // Placeholder calibration settings load, will be read from SAM D21 EEPROM.
    // Consider adding EEPROM to future revision.
    this->_settings = JoystickSettings{{41, 12},
                                       {1023, 1021},
                                       {512, 512},
                                       51, 51, 1.0};

    // Initialize data variable
    this->_data = {{0, 0}, false};
    Keyboard.begin(); // Start keyboard for keystroke capability
}

// Tweak with proper smoothing and deadzone adherence
void TartarusJoystick::readJoystickData() {
    // raw pot X and Y reading
    int16_t xVal, yVal;

    // Read pot values
    xVal = (int16_t) analogRead(A4);
    yVal = (int16_t) analogRead(A3);

    // Apply calibration to X and Y values
    xVal = (int16_t) map(xVal, this->_settings.minPos.x, this->_settings.maxPos.x,
                         0, 1023);

    yVal = (int16_t) map(yVal, this->_settings.minPos.y, this->_settings.maxPos.y,
                         0, 1023);

    this->_applySmoothing(xVal, yVal);
    this->_applyDeadzone(xVal, yVal);

    // Map X and Y values to [-100, 100] with deadzones
    xVal = (int16_t) map(xVal, 0, 1023, JOYSTICK_MINIMUM, JOYSTICK_MAXIMUM);
    yVal = (int16_t) map(yVal, 0, 1023, JOYSTICK_MINIMUM, JOYSTICK_MAXIMUM);

    this->_data.position.x = constrain(xVal, JOYSTICK_MINIMUM, JOYSTICK_MAXIMUM);
    this->_data.position.y = constrain(yVal, JOYSTICK_MINIMUM, JOYSTICK_MAXIMUM);

    this->_data.button = digitalRead(JOYSTICK_BUTTON);
}

void TartarusJoystick::printToSerial() const {
    Serial.print("Position (x, y): ");
    this->_data.position.printToSerial();
    Serial.print(" Button: ");
    Serial.println(this->_data.button);

    Serial.print("Angle (rad): ");
    Serial.println(atan2(this->_data.position.y, this->_data.position.x) + M_PI);
}

// Point printToSerial() implementation
void Point::printToSerial() const {
    Serial.print("(");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(")");
}

void TartarusJoystick::_applyDeadzone(int16_t & xVal, int16_t & yVal) const {
    if(abs(xVal - this->_settings.origin.x) <= this->_settings.xDeadzone) {
        xVal = 512;
    }

    if(abs(yVal - this->_settings.origin.y) <= this->_settings.yDeadzone) {
        yVal = 512;
    }
}

// Scale sensitivity dependent on the distance from the origin
// Range between 0.1 - 2.0.
void TartarusJoystick::_applySmoothing(int16_t & xVal, int16_t & yVal) const {
    xVal = (int16_t)(this->_settings.origin.x +
            (xVal - this->_settings.origin.x) *
            this->_settings.sensitivityScaling);

    yVal = (int16_t)(this->_settings.origin.y +
            (yVal - this->_settings.origin.y) *
            this->_settings.sensitivityScaling);
}
