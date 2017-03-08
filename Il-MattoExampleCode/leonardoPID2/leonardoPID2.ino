#include "PWM.h"
#include "PID.h"
#include "utils.h"
#include "Definitions.h"
PID yawPID(1,0,0);
PID pitchPID(1,0,0);
PID rollPID(1,0,0);

//controller values
float throttle=1000;
float targetYaw;
float targetPitch;
float targetRoll;
//gyro values
float gyroYaw;
float gyroPitch;
float gyroRoll;
//PID output values
float pidYaw;
float pidPitch;
float pidRoll;

void setup() {
  // put your setup code here, to run once:
  init_pwm();
}

void loop() {
  // put your main code here, to run repeatedly:
  //if(mpuInterupt)
  //{
    
    //get gyro data
    gyroYaw = 0;
    gyroPitch = 0;
    gyroRoll = 0;
    
    //get throttle data
        if(throttle<MAX_MOTOR_SPEED) throttle++;
     else throttle=MIN_MOTOR_SPEED;
    targetYaw = 0;
    targetPitch = 0;
    targetRoll = 0;
    
    //apply PID
    pidYaw = yawPID.updatePID(targetYaw, gyroYaw, DELTA_TIME);
    pidPitch = pitchPID.updatePID(targetPitch, gyroPitch, DELTA_TIME);
    pidRoll = rollPID.updatePID(targetRoll, gyroRoll, DELTA_TIME);
    //update motors
    setMotors (throttle, pidYaw, pidPitch, pidRoll);
    _delay_ms(5);
  //}


  //send telemetry
}


