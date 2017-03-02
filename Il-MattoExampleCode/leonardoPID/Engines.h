/*
  Engines.h - Library for controlling a set of Quadcopter engines (aka motors)
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
#ifndef Engines_h
#define Engines_h

#include "Arduino.h"
#include "Definitions.h"
#include "stdint.h"
class Engines
{
  public:
    Engines();
    void init();
    void allStop();
    void setEngineSpeed(int, int, int, int);
    int getEngineSpeed(uint8_t);
    void setThrottle(int);
    
    int getThrottle();
    
    void arm();
    void disarm();
    boolean isArmed();
    
  private:
    
    int engine_speeds[ENGINE_COUNT];

    int throttle;
    
    boolean _armed;
};

#endif
