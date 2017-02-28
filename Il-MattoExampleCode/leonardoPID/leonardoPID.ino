#include "PID.h"
#include "Arduino.h"
#include "stdint.h"
#include "Engines.h"
#include "Definitions.h"
#include "PWM.h"

//functions
void getThrottle();

unsigned long previousTime = 0;
unsigned long currentTime = 0;
unsigned long deltaTime = 0;

PID rollPID = PID(6.1, 0.0, 0.9);
PID pitchPID = PID(6.1, 0.0, 0.9);
PID yawPID = PID(6.0, 0, 0.0);

//all the following can be put in a class to make life easy for getting and storing values

int targetRoll;
int targetPitch;
int targetYaw;

int currentRoll;
int currentPitch;
int currentYaw;

Engines engines;

void setup() {
  // put your setup code here, to run once:
  engines.init();
  init_pwm();
  previousTime = micros();
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i<50; i++)
  {
      currentTime = micros();
      deltaTime = currentTime - previousTime;
      previousTime = currentTime; //reset previous on arming
      getThrottle();
      if (engines.isArmed()){
        if (currentRoll > 90 || currentRoll < -90 || currentPitch > 90 || currentPitch < -90)
        {
          engines.disarm();
        }
    
        float G_Dt = deltaTime / 1000000.0; // Delta time in seconds

          targetRoll =  0;
          targetPitch = 0;
          targetYaw =   0;
    
        // Negative values mean the right side is up
        // Constrain to 45 degrees, because beyond that, we're fucked anyway
        float rollAdjust = rollPID.updatePID(targetRoll, constrain(currentRoll, -50, 50), G_Dt);
    
        // Positive values mean the frontend is up
        // Constrain to 45 degrees, because beyond that, we're fucked anyway
        float pitchAdjust = pitchPID.updatePID(targetPitch, constrain(currentPitch, -50, 50), G_Dt);
    
         // Positive values are to the right
         float headingAdjust = yawPID.updatePID(targetYaw, currentYaw, G_Dt);
    
         // Apply offsets to all motors evenly to ensure we pivot on the center
         int throttle = engines.getThrottle();
         if (throttle > MIN_MOTOR_SPEED){
          engines.setEngineSpeed(throttle - rollAdjust - pitchAdjust + headingAdjust, 
                                 throttle + rollAdjust - pitchAdjust - headingAdjust, 
                                 throttle - rollAdjust + pitchAdjust - headingAdjust, 
                                 throttle + rollAdjust + pitchAdjust + headingAdjust);
         }
         else{
          engines.setEngineSpeed(0,0,0,0);
        }
      }
      else{
        // Reset state
        rollPID.resetError();
        pitchPID.resetError();
        yawPID.resetError();
      } 
    }
  //send telemetry only if engine is armed
}

void getThrottle(){
  // Arm the engines by putting the left stick in the lower-right corner
  //might be worth making a data class to store all values 
  if (1/*getthrrottlefromclass < 50? && getyawfromclass > 1000?*/){
    engines.arm();
  }
  // Disarm the engines by putting the left stick in the lower-left corner
  else if (1/*getthrrottlefromclass< 50 && getyawfromclass < 50*/){
    engines.disarm();
  }
  
  // Process throttle
  engines.setThrottle(/*get throtte from class and normalise to 1000 to 2000 us*/5); // Engines expect throttle to be 0-based
}
//[1]http://robot-kingdom.com/pid-controller-tutorial-for-robots/
//[2]https://github.com/grantmd/QuadCopter




