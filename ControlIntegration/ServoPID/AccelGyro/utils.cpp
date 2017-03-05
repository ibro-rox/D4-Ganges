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
	if(output > 0)
	{	
		output = (output/N_THROTTLE)+1000;
		return output;
	}
	return 0;
}
