#/**
 See http://www.invensense.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
  for the register mappings.


**/
#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"
#include "i2c.h"

// This is all the functions of "i2c.c" from the C7 Lab from last year
void init_i2c_master(void)
{
	/* F_SCL = F_CPU/120 = 100kHz */
	TWBR = 0x34;   // sets the bit rate
	TWSR = 0x00;  // status register
}

void i2c_start(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
	while(!(TWCR & _BV(TWINT)));	
}

void i2c_stop(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}

void i2c_tx(uint8_t b)
{
	TWDR = b;
	TWCR = _BV(TWINT) | _BV(TWEN);
	while(!(TWCR & _BV(TWINT)));
}

uint8_t i2c_rx_ack(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	while(!(TWCR & _BV(TWINT)));
	return TWDR;
}

uint8_t i2c_rx_nack(void)
{
	// twint flag HIGH, Enable I2C
	TWCR = _BV(TWINT) | _BV(TWEN);
	
	// this looks like its waiting for TWINT to go high
	while(!(TWCR & _BV(TWINT)));
	return TWDR;
}
// end of "i2c.c" functions

void readGyro(void)
{
	/*fprintf(stderr,"starting read");
	//select register to read
	i2c_start();
	fprintf(stderr,"start sent ");
	i2c_tx(0xD0); // address 110100; device 0; mode 0 (write)
	fprintf(stderr,"SLA sent");
	i2c_rx_ack();
	fprintf(stderr,"ACK received ");
	i2c_tx(68); // register address; gyro_x_l
	i2c_rx_ack();
	
	i2c_start();
	i2c_tx(0xD1); // same address , read mode (1)
	uint8_t data = i2c_rx_ack();
	//data received
	fprintf(stderr,"data after ACK: ");
	fprintf(stderr,"%d",data);
	fprintf(stderr,"\n");
	uint8_t data2 = i2c_rx_nack();
	//data2 received
	fprintf(stderr,"data after NACK: ");
	fprintf(stderr,"%d",data2);
	fprintf(stderr,"\n");
	
	i2c_stop();
	*/
	
	//FROM THE Il Matto Quick Reference
	
	i2c_start();
	i2c_tx()
	
	
	
}


int main(void)
{
	init_debug_uart0();
	init_i2c_master();
	
	fprintf(stderr,"UART connected\n");
	
	for(;;){
		readGyro();
		fprintf(stderr,"umm\n");
	}

}