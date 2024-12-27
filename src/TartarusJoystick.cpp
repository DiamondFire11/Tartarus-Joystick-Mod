//
// Created by Will Snyder on 12/23/24.
//

#include "includes/TartarusJoystick.h"

TartarusJoystick::TartarusJoystick() {
    // Placeholder calibration settings load, will be read from SAM D21 EEPROM.
    // Consider adding EEPROM to future revision.
    this->m_settings = JoystickSettings{{56, 19},
                                        {1023, 1010},
                                        {571, 526},
                                        51, 51, 1.0};

    // Initialize data variable
    this->m_data = {{0, 0}, false};

    // Initialize keys
    this->m_keys[0] = {NAN, NAN, KEY_BUTTON, false}; // Button is special case
    this->m_keys[1] = {1.17, 5.08, KEY_LEFT, false}; // Left is special case due to singularity
    this->m_keys[2] = {0.39, 2.74, KEY_UP, false};
    this->m_keys[3] = {1.96, 4.30, KEY_RIGHT, false};
    this->m_keys[4] = {3.52, 5.89, KEY_DOWN, false};

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
    xVal = (int16_t) map(xVal, this->m_settings.minPos.x, this->m_settings.maxPos.x,
                         0, 1023);

    yVal = (int16_t) map(yVal, this->m_settings.minPos.y, this->m_settings.maxPos.y,
                         0, 1023);

    this->m_applySmoothing(xVal, yVal);
    this->m_applyDeadzone(xVal, yVal);

    // Map X and Y values to [-100, 100] with deadzones
    xVal = (int16_t) map(xVal, 0, 1023, JOYSTICK_MINIMUM, JOYSTICK_MAXIMUM);
    yVal = (int16_t) map(yVal, 0, 1023, JOYSTICK_MINIMUM, JOYSTICK_MAXIMUM);

    this->m_data.position.x = constrain(xVal, JOYSTICK_MINIMUM, JOYSTICK_MAXIMUM);
    this->m_data.position.y = constrain(yVal, JOYSTICK_MINIMUM, JOYSTICK_MAXIMUM);

    this->m_data.button = digitalRead(JOYSTICK_BUTTON);
}

void TartarusJoystick::printToSerial() const {
    Serial.print("Position (x, y): ");
    this->m_data.position.printToSerial();
    Serial.print(" Button: ");
    Serial.println(this->m_data.button);

    Serial.print("Angle (rad): ");
    Serial.println(atan2(this->m_data.position.y, this->m_data.position.x) + M_PI);
}

// Point printToSerial() implementation
void Point::printToSerial() const {
    Serial.print("(");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(")");
}

void TartarusJoystick::m_applyDeadzone(int16_t & xVal, int16_t & yVal) const {
    if(abs(xVal - this->m_settings.origin.x) <= this->m_settings.xDeadzone) {
        xVal = 512;
    }

    if(abs(yVal - this->m_settings.origin.y) <= this->m_settings.yDeadzone) {
        yVal = 512;
    }
}

// Scale sensitivity dependent on the distance from the origin
// Range between 0.1 - 2.0.
void TartarusJoystick::m_applySmoothing(int16_t & xVal, int16_t & yVal) const {
    xVal = (int16_t)(this->m_settings.origin.x +
                     (xVal - this->m_settings.origin.x) *
                     this->m_settings.sensitivityScaling);

    yVal = (int16_t)(this->m_settings.origin.y +
                     (yVal - this->m_settings.origin.y) *
                     this->m_settings.sensitivityScaling);
}
