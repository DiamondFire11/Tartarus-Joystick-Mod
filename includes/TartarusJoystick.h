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

class TartarusJoystick {
public:
    TartarusJoystick();

    //void publishJoystick() const;
    void publishKeystrokes() const;
    void printToSerial() const;
    void readJoystickData();

    void calibrate();

private:
    JoystickData _data{};
    JoystickSettings _settings{};

    // Calibration helpers
    static void _sampleXData(uint16_t *);
    static void _sampleYData(uint16_t *);
    static double_t _calculateMean(const uint16_t *);
    static double_t _calculateVariance(const double_t &, const uint16_t *);
    static int16_t _findEffectiveRange(const double_t &, const double_t &, bool, const uint16_t *);

    // Smoothing and deadzone helpers
    void _applyDeadzone(int16_t &, int16_t &) const;
    void _applySmoothing(int16_t &, int16_t &) const;

    /*
     * Special helper for saving calibration data
     * Be very careful with how many times the joystick is calibrated
     * DO NOT EXCEED 1000 calibrations over lifespan
     */
    //bool _commitCalibrationToEEPROM();
};
