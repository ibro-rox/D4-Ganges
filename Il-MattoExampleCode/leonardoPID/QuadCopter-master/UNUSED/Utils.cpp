/*
  Utils.cpp - Utility functions for my quadcopter code
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

#include "WProgram.h"

// Insert sort. From "whistler" - http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1283456170/0
void isort(int *a, byte n){
  for (int i = 1; i < n; ++i){
    int j = a[i];
    int k;
    for (k = i - 1; (k >= 0) && (j < a[k]); k--){
      a[k + 1] = a[k];
    }
    a[k + 1] = j;
  }
}

int findMedian(int *data, byte arraySize){
  isort(data, arraySize);
  
  return data[arraySize/2];
}

// Low pass filter, kept as regular C function for speed
float filterSmooth(float currentData, float previousData, float smoothFactor){
  if (smoothFactor != 1.0) //only apply time compensated filter if smoothFactor is applied
    return (previousData * (1.0 - smoothFactor) + (currentData * smoothFactor)); 
  else
    return currentData; //if smoothFactor == 1.0, do not calculate, just bypass!
}
