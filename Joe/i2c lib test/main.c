//I2C test

// must be compiled with twimastertimeout.c


#include "debug.h"
#include <util/delay.h>
#include <avr/io.h>
#include "i2cmaster.h"

#define SLA 110100
#define DEV 0
#define TWI_READ 1
#define TWI_WRITE 0


void start_wrapped(unsigned char address)
{
	if(i2c_start(address))
	{
		// returns 1 if unable to connect
		printf("I2C start failed - device busy\n");
	}
	else
	{
	printf("Device Accessible\n");
	}
}

void write_wrapped(unsigned char data)
{
	if(i2c_write(data))
	{
		// returns 1 if write failed
		printf("Write Failed\n");
	}
	else
	{
		printf("Write Success\n");
	}	
}
	


int main(void) {
	
	init_debug_uart0();
	i2c_init();
	printf("UART Connected\n");
	
	// therefore device address is 0xD1 for a read
	//							   0xD0 for a write
	
	unsigned char data;
	
	for(;;)
	{
		start_wrapped(0xD0); // write mode
		write_wrapped(0x42); // selects gyro_x_low
		start_wrapped(0xD1); // read mode, now register is selected
		data = i2c_readNak(); // read a byte
		i2c_stop(); // release the bus
		printf("Data: %d\n\n", (int)data);
		_delay_ms(1000);		
	}
	
}