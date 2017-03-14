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
//#include "rfm12_config.h"
#include "drone_comms.h"

int main(void)
{
	// Initialise rfm12 and interrupts
	init_uart1();
	rfm12_init();
	sei();
	send_string("Initialised");

	uint8_t receivedpackettype;
	uint16_t receiveddata, i;

	uint16_t thrust, roll, pitch, yaw;

	char ch[30];
	while (1)
	{
		rfm12_tick();

		// Wait for data to be fully received
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			//i++;
			// Get the received packet type and data
			receivedpackettype = rfm12_rx_type();
			receiveddata = *rfm12_rx_buffer();

			rfm12_rx_clear();
			// Decrypt (if enabled) and extract 10-bit data and packet type from the received packet 
			Retrieve_data(&receivedpackettype, &receiveddata);
			
			//sprintf(ch, "\n\rReceived: %u %u", receivedpackettype, receiveddata);
			//send_string(ch);

#if UPLINK_TEST
			//send_string("\n\rReceived transmission:");
			// Send received packet type to UART
			//sprintf(ch,"\n\r Packet type: %u",receivedpackettype);
			//send_string(ch);
			// Send received data to UART
			//sprintf(ch, "\n\r Data = %u", receiveddata);
			//send_string(ch);

			//if (receiveddata > 1000) break;
#endif
#if ENABLE_CONTROLS
			switch (receivedpackettype)
			{
			case OP_THRUST:
				thrust = receiveddata;

				sprintf(ch, "\n\r Thrust = %u", thrust);
				send_string(ch);
				break;
			case OP_ROLL:
				roll = receiveddata;

				sprintf(ch, " Roll = %u", roll);
				send_string(ch);
				break;
			case OP_YAW:
				yaw = receiveddata;

				sprintf(ch, " Yaw = %u", yaw);
				send_string(ch);
				break;
			case OP_PITCH:
				pitch = receiveddata;

				sprintf(ch, " Pitch = %u", pitch);
				send_string(ch);
				break;
			}
#endif

		}
	}

	while (1)
	{

	}
}