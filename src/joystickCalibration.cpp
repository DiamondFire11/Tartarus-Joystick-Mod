//
// Created by Will Snyder on 12/23/24.
//

#include "includes/TartarusJoystick.h"

/*
 * Joystick calibration. Uses mean and variance to find accurate
 * joystick min and max values. Filters for outliers that may occur due
 * to ADC error
 *
 * Total time of process approx 30 seconds
 */
void TartarusJoystick::calibrate() {
    uint32_t time;
    uint16_t readings[CALIBRATION_SAMPLES] = {0};
    double_t mean, variance;

    // Remove old calibration settings
    this->m_settings.minPos = {512, 512};
    this->m_settings.maxPos = {512, 512};

    time = millis();
    Serial.println("Beginning joystick calibration");
    Serial.println("Move joystick to -X position");

    digitalWrite(LED_BUILTIN, HIGH);
    while((millis() - time) < 3000) {} // Wait 3s for user to move joystick
    digitalWrite(LED_BUILTIN, LOW);

    // Calibrate X minimum
    TartarusJoystick::m_sampleXData(readings);
    mean = TartarusJoystick::m_calculateMean(readings);
    variance = TartarusJoystick::m_calculateVariance(mean, readings);
    this->m_settings.minPos.x = TartarusJoystick::m_findEffectiveRange(mean, variance, false, readings);

    time = millis();
    Serial.println("Move joystick to +X position");
    digitalWrite(LED_BUILTIN, HIGH);
    while((millis() - time) < 3000) {} // Wait 3s for user to move joystick
    digitalWrite(LED_BUILTIN, LOW);

    // Calibrate X maximum
    TartarusJoystick::m_sampleXData(readings);
    mean = TartarusJoystick::m_calculateMean(readings);
    variance = TartarusJoystick::m_calculateVariance(mean, readings);
    this->m_settings.maxPos.x = TartarusJoystick::m_findEffectiveRange(mean, variance, true, readings);

    time = millis();
    Serial.println("Move joystick to -Y position");
    digitalWrite(LED_BUILTIN, HIGH);
    while((millis() - time) < 3000) {} // Wait 3s for user to move joystick
    digitalWrite(LED_BUILTIN, LOW);

    // Calibrate Y minimum
    TartarusJoystick::m_sampleYData(readings);
    mean = TartarusJoystick::m_calculateMean(readings);
    variance = TartarusJoystick::m_calculateVariance(mean, readings);
    this->m_settings.minPos.y = TartarusJoystick::m_findEffectiveRange(mean, variance, false, readings);

    time = millis();
    Serial.println("Move joystick to +Y position");
    digitalWrite(LED_BUILTIN, HIGH);
    while((millis() - time) < 3000) {} // Wait 3s for user to move joystick
    digitalWrite(LED_BUILTIN, LOW);

    // Calibrate Y maximum
    TartarusJoystick::m_sampleYData(readings);
    mean = TartarusJoystick::m_calculateMean(readings);
    variance = TartarusJoystick::m_calculateVariance(mean, readings);
    this->m_settings.maxPos.y = TartarusJoystick::m_findEffectiveRange(mean, variance, true, readings);

    time = millis();
    Serial.println("Remove finger from joystick while origin is calibrated");
    digitalWrite(LED_BUILTIN, HIGH);
    while((millis() - time) < 3000) {} // Wait 3s for user to move joystick
    digitalWrite(LED_BUILTIN, LOW);

    // Calibrate origin
    TartarusJoystick::m_sampleXData(readings);
    this->m_settings.origin.x = (int16_t) TartarusJoystick::m_calculateMean(readings);

    TartarusJoystick::m_sampleYData(readings);
    this->m_settings.origin.y = (int16_t) TartarusJoystick::m_calculateMean(readings);

    Serial.print("X Min: ");
    Serial.println(this->m_settings.minPos.x);
    Serial.print("X Max: ");
    Serial.println(this->m_settings.maxPos.x);

    Serial.print("Y Min: ");
    Serial.println(this->m_settings.minPos.y);
    Serial.print("Y Max: ");
    Serial.println(this->m_settings.maxPos.y);

    Serial.print("Origin: (");
    Serial.print(this->m_settings.origin.x);
    Serial.print(", ");
    Serial.print(this->m_settings.origin.y);
    Serial.println(")");
}

void TartarusJoystick::m_sampleXData(uint16_t * samplesArr) {
    for(auto i = 0; i < CALIBRATION_SAMPLES; ++i){
        samplesArr[i] = analogRead(A4);
        delay(10);
    }
}

void TartarusJoystick::m_sampleYData(uint16_t * samplesArr) {
    for(auto i = 0; i < CALIBRATION_SAMPLES; ++i){
        samplesArr[i] = analogRead(A3);
        delay(10);
    }
}

// The mean describes the value that you can expect from a distribution
// This value is important for calculating variance
double_t TartarusJoystick::m_calculateMean(const uint16_t * samplesArr) {
    uint32_t sum = 0;
    for(auto i = 0; i < CALIBRATION_SAMPLES; ++i){
        sum += samplesArr[i];
    }
    return (double_t) sum / CALIBRATION_SAMPLES;
}

// Variance describes how far a reading deviates from the calculated mean
// Will be used to filter outliers in the calibration
double_t TartarusJoystick::m_calculateVariance(const double_t & mean, const uint16_t * samplesArr) {
    double_t variance = 0;
    for(auto i = 0; i < CALIBRATION_SAMPLES; ++i){
        variance += pow(((double_t) samplesArr[i] - mean), 2);
    }
    return variance / (CALIBRATION_SAMPLES - 1);
}

// The bool (opt) parameter determines if you will be finding the min or max.
// opt = false  -   Find the minimum in the set
// opt = true   -   Find the maximum in the set
int16_t TartarusJoystick::m_findEffectiveRange(const double_t & mean, const double_t & variance,
                                              const bool opt, const uint16_t * samplesArr) {
    double_t outlierThreshold = sqrt(variance) * 2; // Two times the standard deviation
    int16_t min = 512;
    int16_t max = 512;
    for(auto i = 0; i < CALIBRATION_SAMPLES; ++i){
        if(opt){ // Find max
            if(abs(samplesArr[i] - mean) <= outlierThreshold && samplesArr[i] > max){
                max = (int16_t) samplesArr[i];
            }
        } else { // Find min
            if(abs(samplesArr[i] - mean) <= outlierThreshold && samplesArr[i] < min){
                min = (int16_t) samplesArr[i];
            }
        }
    }
    return (opt ? max : min);
}
