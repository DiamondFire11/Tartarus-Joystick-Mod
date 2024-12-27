//
// Created by Will Snyder on 12/23/24.
//

#pragma once

// Std C Library
#include <cstdint>

// Arduino Headers
#include <Arduino.h>
#include "Keyboard.h"

// Keyboard key settings
enum JoystickKeys {
    KEY_UP = 'w',
    KEY_DOWN = 's',
    KEY_LEFT = 'a',
    KEY_RIGHT = 'd',
    KEY_BUTTON = KEY_ESC
};

// Structure for representing joystick position in R2
typedef struct Point {
    int16_t x;
    int16_t y;
    void printToSerial() const;
} Point;

// Structure to cleanly hold the data pertaining to the joystick
typedef struct JoystickData {
    Point position;
    bool button;
} JoystickData;

struct JoystickSettings {
    // Calibration settings
    Point minPos; // Coordinate pair for the minimum pot analog value
    Point maxPos; // Coordinate pair for the maximum pot analog value
    Point origin; // Logical "origin" of joystick (when upright)

    // Deadzones settings
    uint16_t xDeadzone;
    uint16_t yDeadzone;

    // Sensitivity
    double sensitivityScaling;
};

enum ANALOG_VALUES{
    JOYSTICK_MINIMUM = -100,
    JOYSTICK_MAXIMUM = 100,
    CALIBRATION_SAMPLES = 100};

typedef struct KeySettings {
    double_t rangeMin;
    double_t rangeMax;
    JoystickKeys key;
    bool flag;
} KeySettings;

class TartarusJoystick {
public:
    TartarusJoystick();

    //void publishJoystick() const;
    void publishKeystrokes();
    void printToSerial() const;
    void readJoystickData();

    void calibrate();

private:
    JoystickData m_data{};
    JoystickSettings m_settings{};
    KeySettings m_keys[5];

    // Calibration helpers
    static void m_sampleXData(uint16_t *);
    static void m_sampleYData(uint16_t *);
    static double_t m_calculateMean(const uint16_t *);
    static double_t m_calculateVariance(const double_t &, const uint16_t *);
    static int16_t m_findEffectiveRange(const double_t &, const double_t &, bool, const uint16_t *);

    // Smoothing and deadzone helpers
    void m_applyDeadzone(int16_t &, int16_t &) const;
    void m_applySmoothing(int16_t &, int16_t &) const;

    /*
     * Special helper for saving calibration data
     * Be very careful with how many times the joystick is calibrated
     * DO NOT EXCEED 1000 calibrations over lifespan
     */
    //bool m_commitCalibrationToEEPROM();
};
