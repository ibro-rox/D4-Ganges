/*
	drone_comms.h
	Header file for drone_comms.c
	Author: Joel Trickey
*/
#include "../comms.h"

void Decode_data(uint8_t* type, uint16_t* data, uint16_t totalpacket);
void Retrieve_data(uint8_t* type, uint16_t* data);
uint16_t Decrypt_data(uint16_t packet);