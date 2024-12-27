//
// Created by Will Snyder on 12/26/24.
//

#include "includes/TartarusJoystick.h"

void TartarusJoystick::publishKeystrokes() {
    double_t angle = atan2(this->m_data.position.y, this->m_data.position.x) + M_PI; // Range between 0 and 2pi

    // Handle button press
    if(this->m_data.button){
        if(!this->m_keys[0].flag) {
            Keyboard.press(m_keys[0].key);
            this->m_keys[0].flag = true;
        }

    } else {
        if(this->m_keys[0].flag) {
            Keyboard.release(m_keys[0].key);
            this->m_keys[0].flag = false;
        }
    }

    // If in deadzone release keys and return
    if(this->m_data.position.x == 0 && this->m_data.position.y == 0) {
        Keyboard.release(JoystickKeys::KEY_UP);
        Keyboard.release(JoystickKeys::KEY_DOWN);
        Keyboard.release(JoystickKeys::KEY_LEFT);
        Keyboard.release(JoystickKeys::KEY_RIGHT);

        // Reset key tracking flags
        this->m_keys[1].flag = false;
        this->m_keys[2].flag = false;
        this->m_keys[3].flag = false;
        this->m_keys[4].flag = false;
        return;
    }

    // Joystick in 'A' region (has to account for coordinate singularity)
    if((0 <= angle && angle <= this->m_keys[1].rangeMin) || (this->m_keys[1].rangeMax < angle && angle <= 2*M_PI)) {
        if(!this->m_keys[1].flag) {
            Keyboard.press(m_keys[1].key);
            this->m_keys[1].flag = true;
        }

    } else {
        if(this->m_keys[1].flag) {
            Keyboard.release(m_keys[1].key);
            this->m_keys[1].flag = false;
        }
    }

    for(auto i = 2; i < 5; ++i) {
        if(this->m_keys[i].rangeMin < angle && angle <= this->m_keys[i].rangeMax) {
            if(!this->m_keys[i].flag) {
                Keyboard.press(m_keys[i].key);
                this->m_keys[i].flag = true;
            }

        } else {
            if (this->m_keys[i].flag) {
                Keyboard.release(m_keys[i].key);
                this->m_keys[i].flag = false;
            }
        }
    }
}
