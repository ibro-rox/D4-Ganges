#include <avr/io.h>
#include "dl_comms_1.h"

int main()
{
	init_spi();

	
}

void init_spi()
{
	DDRB = _BV(PB4) | _BV(PB5) | _BV(PB7);
	SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPI2X);
}

void transmit(uint8_t data)
{
	SPDR = data;
}