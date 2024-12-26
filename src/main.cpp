#include <Arduino.h>

#include "includes/joystickFSM.h"        // Joystick Finite State Machine
#include "includes/ledFSM.h"             // LED Finite State Machine

typedef struct Task {
    int state;
    uint32_t period;
    uint32_t elapsedTime;
    int (*tickFcn) (int);
} Task;

Task tasks[2];

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(JOYSTICK_BUTTON, INPUT);

    Serial.begin(9600); // Start USB Serial
    Serial.println("Hello from Tartarus Joystick Mod M0 v1.0");

    // LED State-Machine setup
    tasks[0].state = LEDStates::LED_INIT;
    tasks[0].period = 500;
    tasks[0].elapsedTime = 0;
    tasks[0].tickFcn = &ledTick;

    // Joystick State-Machine setup
    tasks[1].state = JoystickStates::JOYSTICK_INIT;
    tasks[1].period = 5;
    tasks[1].elapsedTime = 0;
    tasks[1].tickFcn = &joystickTick;
}

void loop() {
    for (auto & task : tasks) {
        if ((millis() - task.elapsedTime) >= task.period) {
            task.state = task.tickFcn(task.state);
            task.elapsedTime = millis(); // Last time this task was ran
        }
    }
}
