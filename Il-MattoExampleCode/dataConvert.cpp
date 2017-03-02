#include <stdio.h>

#define N_ANGLE 51.1
#define N_THROTTLE 0.511
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

int main(void)
{

	int yaw = 1023;
	int pitch = 0;
	int roll = 512;
	int throttle = 400;
	printf("raw input: %d yaw angle: %f\n",yaw, rawToAngle(yaw));
	printf("raw input: %d pitch angle: %f\n", pitch, rawToAngle(pitch));
	printf("raw input: %d roll angle: %f\n",roll, rawToAngle(roll));
	printf("raw input: %d throttle 1: %f\n", throttle, rawToThrottle(throttle));
	throttle = 1023;
	printf("raw input: %d throttle 2: %f\n", throttle, rawToThrottle(throttle));
}