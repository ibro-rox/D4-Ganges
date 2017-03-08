// This code involves taking a copy from drone_comms.c and simply printing off the received data off putty
// Arthur: Joel And Mohammed
// Date : 3/1/17
// this code is also for test 2 im mentioned in my pink book (receiving the PID values)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>// for sprintf()
#include "rfm12.h"
#include "drone_comms.h"

int main(void)
{	
	// Initialise rfm12 and interrupts
	rfm12_init();
	init_uart1();
	sei();
	send_string("\n\rInitialised\n\r");

	uint8_t receivedpackettype, PIDmode;
	uint16_t receiveddata;
	PIDmode = 1;
	char ch[30];
	float k_value;
	while (1)
	{
		rfm12_tick();
		
		// Wait for data to be fully received
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			// Get the received packet type and data
			receivedpackettype = rfm12_rx_type();
			receiveddata = rfm12_rx_buffer();
			rfm12_rx_clear();

			sprintf(ch, "\n\r Raw data: %u %u", receivedpackettype, receiveddata);
			send_string(ch);
			// Decrypt (if enabled) and extract 10-bit data and packet type from the received packet 
			Retrieve_data(&receivedpackettype, &receiveddata);
			sprintf(ch, "\n\r Received: %u %u", receivedpackettype, receiveddata);
			send_string(ch);
			 #if UPLINK_TEST
			 	send_string("\n\rReceived transmission:");
			 	// Send received packet type to UART
			 	sprintf(ch,"\n\r Packet type: %d",receivedpackettype);
			 	send_string(ch);
			 	// Send received data to UART
			 	sprintf(ch, "\n\r Data = %d", receiveddata);
			 	send_string(ch);
			 #endif

#if ENABLE_CONTROLS
			switch (receivedpackettype)
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
				default : send_string("Didnt receive K values");
			}
#endif
#if ENABLE_UI
			// If in PID mode also check for K values
			if (PIDmode)
			{
				switch (receiveddata)
				{
					case OP_KP_ROLL:
						k_value = receiveddata / 100;
						sprintf(ch, "Roll Kp value = %f", k_value);
						send_string(ch);
						break;
					case OP_KI_ROLL:
						k_value = receiveddata / 100;
						sprintf(ch, "Roll Ki value = %f", k_value);
						send_string(ch);
						break;
					case OP_KD_ROLL:
						k_value = receiveddata / 100;
						sprintf(ch, "Roll Kd value = %f", k_value);
						send_string(ch);
						break;
					case OP_KP_YAW:
						k_value = receiveddata / 100;
						sprintf(ch, "Yaw Kp value = %f", k_value);
						send_string(ch);
						break;
					case OP_KI_YAW:
						k_value = receiveddata / 100;
						sprintf(ch, "Yaw Ki value = %f", k_value);
						send_string(ch);
						break;
					case OP_KD_YAW:
						k_value = receiveddata / 100;
						sprintf(ch, "Yaw Kd value = %f", k_value);
						send_string(ch);
						break;
					case OP_KP_PITCH:
						k_value = receiveddata / 100;
						sprintf(ch, "Pitch Kp value = %f", k_value);
						send_string(ch);
						break;
					case OP_KI_PITCH:
						k_value = receiveddata / 100;
						sprintf(ch, "Pitch Ki value = %f", k_value);
						send_string(ch);
						break;
					case OP_KD_PITCH:
						k_value = receiveddata / 100;
						sprintf(ch, "Pitch Kd value = %f", k_value);
						send_string(ch);
						break;
				}
			}
#endif // ENABLE_UI
		}
	}
}