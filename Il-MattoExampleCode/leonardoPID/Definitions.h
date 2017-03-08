/*
  Global definitions for my quadcopter. Holds constants and config flags for either globally
    useful data or things I might want to change easily. There may be other definitions elsewhere
    in the codebase!
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

// Global definitions
#define ENGINE_COUNT 4
// Speeds are in microseconds, not degrees, for greater control
#define MIN_MOTOR_SPEED 1000
#define MAX_MOTOR_SPEED 2000

#define LEFT_FRONT_MOTOR 0
#define RIGHT_FRONT_MOTOR  1
#define LEFT_REAR_MOTOR 2
#define RIGHT_REAR_MOTOR 3


#define ROLL 0
#define PITCH 1
#define YAW 2

#define XAXIS 0
#define YAXIS 1
#define ZAXIS 2

#define TIMER_TOP 40000
#define PWM_DUTY_MIN 2000
#define PWM_DUTY_MAX 4000
