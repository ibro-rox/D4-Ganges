#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "rfm12.h"
#include "basestation_comms.h"

//Interrupt for ADC conversion complete => send data to the drone
/*
	ISR()
	{
		Send_data(Datatype type, data);
		
	}
*/

void Send_data(Datatype type, uint8_t data);
{
	rfm12_tx(sizeof(data), type, &data);
}

typedef enum {Thrust = 0, Roll, Yaw, SecCheck} Datatype;

int main(void)
{
	// Initialise rfm12 and interrupts
	rfm12_init();
	sei();

	// Send test data
	uint8_t testdata = 0;
	rfm12_tx(sizeof(testdata), 0xFF, &testdata);

	while (1)
	{
		rfm12_tick();
		
		// ### CODE FOR TEST ###
		testdata++;
		_delay_ms(1000);
		rfm12_tx(sizeof(testdata), 0xFF, &testdata);
		// #######
	}
}