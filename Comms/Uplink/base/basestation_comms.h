/*
	basestation_comms.h
	Header file for basestation_comms.c
	Author: Joel Trickey
*/
#include "../comms.h"

void Send_data(uint8_t type, uint16_t data);
void Encode_data(uint8_t* type, uint8_t* data, uint16_t totalpacket);
uint16_t Encrypt_data(uint16_t packet);