#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "rfm12.h"
#include "basestation_comms.h"

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

void Send_data(uint8_t type, uint16_t data)
{
	Encode_data(&type, &data, data);
	rfm12_tx(sizeof(data), type, &data);
}

void Encode_data(uint8_t* type, uint8_t* data, uint16_t ADCOutput)
{
	// Get 8 LSBs from the 10-bit ADC output
	uint8_t lsb8;
	lsb8 = ADCOutput;
	*data = lsb8;

	// Get 2 MSBs from the 10-bit ADC output
	uint16_t msb2;
	msb2 = ADCOutput - lsb8;
	msb2 = msb2 >> 8;

	// Shift packet type to the left and shift in 2 MSB bits
	*type = (2 << 2) + msb2;
}