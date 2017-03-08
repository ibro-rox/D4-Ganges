#include "../comms.h"

// Include the uart init() , transmit() and send_string() for printing data / for debuggig purpose
#define BAUD 57600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

void Send_data(uint8_t type, uint16_t data);
void Retrieve_data(uint8_t* type, uint16_t* data);

void Encode_data(uint8_t* type, uint8_t* data, uint16_t totalpacket);
void Decode_data(uint8_t* type, uint16_t* data, uint16_t totalpacket);

#if ENABLE_ENCRYPTION
uint16_t Encrypt_data(uint16_t packet);
uint16_t Decrypt_data(uint16_t packet); 
uint16_t get_1s(uint8_t num);
#endif

void init_uart1();
void uart_transmit(char data);
void send_string(char *str);