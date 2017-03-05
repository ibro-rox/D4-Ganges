// This code involves taking a copy from drone_comms.c and simply printing off the received data off putty
// Arthur: Joel And Mohammed
// Date : 3/1/17
// this code is also for test 1 im mentioned in my pink book
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>// for sprintf()
#include <math.h>

#include "rfm12.h"
#include "drone_comms.h"

int main(void)
{	
	// Initialise rfm12 and interrupts
	init_uart1();
	rfm12_init();
	sei();
	send_string("Initialised");

	uint8_t receivedpackettype;
	uint16_t receiveddata;
	char ch[30];
	while (1)
	{
		rfm12_tick();
		rfm12_poll();
		// Wait for data to be fully received
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			// Get the received packet type and data
			receivedpackettype = rfm12_rx_type();
			receiveddata = *rfm12_rx_buffer();
			sprintf(ch, "\n\rReceived: %u %u", receivedpackettype, receiveddata);
			send_string(ch);

			// Decrypt (if enabled) and extract 10-bit data and packet type from the received packet 
			Retrieve_data(&receivedpackettype, &receiveddata);
			#if UPLINK_TEST
				//send_string("\n\rReceived transmission:");
				// Send received packet type to UART
				//sprintf(ch,"\n\r Packet type: %u",receivedpackettype);
				//send_string(ch);
				// Send received data to UART
				//sprintf(ch, "\n\r Data = %u", receiveddata);
				//send_string(ch);

				rfm12_rx_clear();
			#endif
			#if ENABLE_CONTROLS
				switch(receivedpackettype)
				{
					case OP_THRUST : 
						sprintf(ch, "\n\r Thrust = %u", receiveddata);
						send_string(ch);
						break;
					case OP_ROLL : 
						sprintf(ch, "\n\r Roll = %u", receiveddata);
						send_string(ch);
						break;
					case OP_YAW :
						sprintf(ch, "\n\r Yaw = %u", receiveddata);
						send_string(ch);
						break;
					case OP_PITCH : 
						sprintf(ch, "\n\r Pitch = %u", receiveddata);
						send_string(ch);
						break;
				}
			#endif

			// print off the adc data

		}
	}
}