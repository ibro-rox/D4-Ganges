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
#include "rfm12_config.h"
#include "drone_comms.h"

int main(void)
{	
	// Initialise rfm12 and interrupts
	init_uart1();
	rfm12_init();
	sei();
	send_string("\n\rInitialised");

	uint8_t receivedpackettype, i;
	uint16_t receiveddata;
	i = 0;
	char ch[30];
	sprintf(ch, "Data rate %ul", ((uint8_t)((10000000.0 / 29.0 / 115200) - 0.5)));
	//send_string(ch);
	while (1)
	{
		//rfm12_tick();
		//rfm12_poll();
		// Wait for data to be fully received
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			if (i == 0)
			{
				i = 1;
				send_string("Rx started");
			}
			// Get the received packet type and data
			receivedpackettype = rfm12_rx_type();
			receiveddata = *rfm12_rx_buffer();
			rfm12_rx_clear();
			//sprintf(ch, "\n\rRaw received data: %u %u", receivedpackettype, receiveddata);
			//send_string(ch);

			// Decrypt (if enabled) and extract 10-bit data and packet type from the received packet 
			Retrieve_data(&receivedpackettype, &receiveddata);
			sprintf(ch, "\n\rDecoded data: %u %u", receivedpackettype, receiveddata);
			send_string(ch);
			if (receiveddata == 1022) break;
			#if UPLINK_TEST
				//send_string("\n\rReceived transmission:");
				// Send received packet type to UART
				//sprintf(ch,"\n\r Packet type: %u",receivedpackettype);
				//send_string(ch);
				// Send received data to UART
				//sprintf(ch, "\n\r Data = %u", receiveddata);
				//send_string(ch);

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
	send_string("Rx finished");
	while(1)
	{ }
}