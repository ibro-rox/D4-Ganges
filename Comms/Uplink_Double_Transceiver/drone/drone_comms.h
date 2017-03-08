#include "../comms.h"

void Send_data(uint8_t type, uint16_t data);
void Encode_data(uint8_t* type, uint8_t* data, uint16_t totalpacket);
uint16_t Encrypt_data(uint16_t packet);

void Decode_data(uint8_t* type, uint16_t* data, uint16_t totalpacket);
void Retrieve_data(uint8_t* type, uint16_t* data);
uint16_t Decrypt_data(uint16_t packet);

void send_string(char *str);
void uart_transmit(char data);
void init_uart1();