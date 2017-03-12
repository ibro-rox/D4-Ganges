#include "utils.h"
#include "Definitions.h"
float rawToAngle(int controlIn)
{
	float output = controlIn - 512;
	output = output/N_ANGLE;
	return output;
}

float rawToThrottle(int controlIn)
{
	float output = controlIn - 512;
  controlIn = (controlIn > MAX_THROTTLE_IN )? MAX_THROTTLE_IN : controlIn;
	if(output > 4)
	{	
		return output;
	}
	return 0;
}
