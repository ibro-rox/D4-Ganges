#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "rfm12.h"
#include "drone_comms.h"

int main(void)
{	
	// Initialise rfm12 and interrupts
	rfm12_init_down();
	sei();

	uint8_t receivedpackettype;
	uint16_t receiveddata;

	while (1)
	{
		rfm12_tick(0);
		
		// Wait for data to be fully received
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{	
			receivedpackettype = rfm12_rx_type();
			receiveddata = rfm12_rx_buffer();
			rfm12_rx_clear();
			// Extract 10-bit ADC value and packet type from the received packet
			Decode_data(&receivedpackettype, &receiveddata);
		}
	}
}

uint16_t Decode_data(uint8_t* packettype, uint16_t* data)
{
	// Get full 16 bits
	uint16_t totalpacket;
	totalpacket = (*packettype << 8) + data;

	// Get 10-bit data from the 16 bit
	*data = totalpacket & (uint16_t)1023;
	
	// Get packet type
	*packettype = *data >> 10;
}

// Send relevant data to the control Il Matto
/*
void Send_controls()
{

}
*/