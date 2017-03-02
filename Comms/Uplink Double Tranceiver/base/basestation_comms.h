//typedef enum { Thrust = 0, Roll, Yaw, SecCheck, Button } Datatype;
#include "../comms.h"

void Send_data(uint8_t type, uint8_t data);
void Encode_data(uint8_t* type, uint8_t* data, uint16_t ADCOutput);