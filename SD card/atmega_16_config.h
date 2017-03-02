
#ifndef _ATMEGA16_CONFIG_
#define _ATMEGA16_CONFIG_

#ifndef F_CPU
#define F_CPU 8000000
#endif

#define LCD
#define rs PA0
#define rw PA1
#define en PA2

#define USART
#define USART_BAUDRATE 9600
#define SERIAL_DEBUG

#define SPI
#define SDSC

#endif
