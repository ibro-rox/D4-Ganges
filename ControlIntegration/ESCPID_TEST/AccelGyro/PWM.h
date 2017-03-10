#ifndef PWM_h
#define PWM_h
#include "Definitions.h"
#include "Arduino.h"
void init_pwm(void);
void pwm_duty(uint8_t motor,float duty);
void setMotors (float throttle, float yaw, float pitch, float roll);

#endif
