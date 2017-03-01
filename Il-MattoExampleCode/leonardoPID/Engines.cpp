/*
  Engines.cpp - Library for controlling a set of Quadcopter engines (aka motors)
  Created by Myles Grant <myles@mylesgrant.com>
  See also: https://github.com/grantmd/QuadCopter
  
  This program is free software: you can redistribute it and/or modify 
  it under the terms of the GNU General Public License as published by 
  the Free Software Foundation, either version 3 of the License, or 
  (at your option) any later version. 

  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
  GNU General Public License for more details. 

  You should have received a copy of the GNU General Public License 
  along with this program. If not, see <http://www.gnu.org/licenses/>. 
*/

#include "Engines.h"
#include "Definitions.h"
#include "stdint.h"
  #include "Arduino.h"
#include "PWM.h"
Engines::Engines(){  
  // Setup engines
}

void Engines::init(){
  allStop();
  _armed =false;
}

void Engines::allStop(){
  throttle = 0;
  setEngineSpeed(0,0,0,0);
}

void Engines::setEngineSpeed(int speed_A, int speed_B, int speed_C, int speed_D){
  speed_A += MIN_MOTOR_SPEED;
  speed_B += MIN_MOTOR_SPEED;
  speed_C += MIN_MOTOR_SPEED;
  speed_D += MIN_MOTOR_SPEED;
  speed_A = constrain(speed_A, MIN_MOTOR_SPEED, MAX_MOTOR_SPEED);
  speed_B = constrain(speed_B, MIN_MOTOR_SPEED, MAX_MOTOR_SPEED);
  speed_C = constrain(speed_C, MIN_MOTOR_SPEED, MAX_MOTOR_SPEED);
  speed_D = constrain(speed_D, MIN_MOTOR_SPEED, MAX_MOTOR_SPEED);
  // Analog write supports commands from 0-255 => 0 - 100% duty cycle
  // Using 125-250 for motor setting 1000-2000
  //analogWrite(engines[engine], speed / 8);
  //convert to pwm and output
    //pwm_duty(2*speed_A,2*speed_B,2*speed_C, 2* speed_D);

    engine_speeds[LEFT_FRONT_MOTOR] = speed_A;
    engine_speeds[RIGHT_FRONT_MOTOR] = speed_B;
    engine_speeds[LEFT_REAR_MOTOR] = speed_C;
    engine_speeds[RIGHT_REAR_MOTOR] = speed_D;
}

int Engines::getEngineSpeed(uint8_t engine){
  return engine_speeds[engine];
}

// Increase/decrease throttle. Flight Control takes care of applying this to the engines
void Engines::setThrottle(int new_throttle){
  throttle = new_throttle;
}

int Engines::getThrottle(){
  return throttle;
}

void Engines::arm(){
  if (_armed) return;
  setEngineSpeed(MIN_MOTOR_SPEED,MIN_MOTOR_SPEED,MIN_MOTOR_SPEED,MIN_MOTOR_SPEED);

  _armed = 1;
}

void Engines::disarm(){
  if (!_armed) return;
  
  setThrottle(0);
  allStop();
  
  _armed = 0;
}

boolean Engines::isArmed(){
  return _armed;
}
