// This code involves taking a copy from drone_comms.c and simply printing off the received data off putty
// Arthur: Joel And Mohammed
// Date : 3/1/17
// this code is also for test 1 im mentioned in my pink book
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>// for sprintf()
#include <stdlib.h>
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

#if ENABLE_UI
	uint8_t PIDmode;
	PIDmode = 1;
#endif

	uint8_t receivedpackettype, cargohookup;
	cargohookup = 1;
	uint16_t receiveddata, i;
	i = 0;
	char ch[30];
	float k_value;
	while (1)
	{
		rfm12_tick();
		//rfm12_poll();
		//receivedpackettype = rfm12_read_fifo_inline();
		//sprintf(ch, "\n\rFIFO: %u", receivedpackettype);
		//if (receivedpackettype != 0) send_string(ch);
		// Wait for data to be fully received
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			i++;
			sprintf(ch, "i = %u", i);
			send_string(ch);

			// Get the received packet type and data
			receivedpackettype = rfm12_rx_type();
			receiveddata = *rfm12_rx_buffer();
			//sprintf(ch, "\n\rReceived: %u %u", receivedpackettype, receiveddata);
			//send_string(ch);
			rfm12_rx_clear();

			// Decrypt (if enabled) and extract 10-bit data and packet type from the received packet 
			Retrieve_data(&receivedpackettype, &receiveddata);
			
			//sprintf(ch, "\n\rDecrypted %u %u", receivedpackettype, receiveddata);
			//send_string(ch);

#if UPLINK_TEST
			sprintf(ch, "\n\rReceived: %u %u", receivedpackettype, receiveddata);
			send_string(ch);
			//if (receiveddata > 1000) break;
#endif
#if ENABLE_CONTROLS
			switch (receivedpackettype)
			{
			case OP_THRUST:
				sprintf(ch, "\n\r Thrust = %u", receiveddata);
				//send_string(ch);
				break;
			case OP_ROLL:
				sprintf(ch, " Roll = %u", receiveddata);
				//send_string(ch);
				break;
			case OP_YAW:
				sprintf(ch, " Yaw = %u", receiveddata);
				//send_string(ch);
				break;
			case OP_PITCH:
				sprintf(ch, " Pitch = %u", receiveddata);
				//send_string(ch);
				break;
			case OP_BUTTON:
				switch (receiveddata)
				{
				case BTN_CARGO_HOOK_UP:
					send_string("\n\rCargo hook up\n\r");
					break;
				case BTN_CARGO_HOOK_DOWN:
					send_string("\n\rCargo hook down\n\r");
					break;
#if ENABLE_UI				
				case BTN_FLIGHT_MODE:
					send_string("\n\rEntering flight mode\n\r");
					PIDmode = 0;
					break;
#endif // ENABLE_UI

#if ENABLE_UI
				case BTN_PID_MODE:
					send_string("\n\rEntering PID mode\n\r");
					PIDmode = 1;
					break;
#endif // ENABLE_UI
				}
				break;
			}
#endif // ENABLE_CONTROLS

#if ENABLE_UI
			if (PIDmode == 1)
			{
				switch (receivedpackettype)
				{
				case OP_KP_ROLL:
					k_value = (float) receiveddata / 100;
					sprintf(ch, "\n\rRoll Kp value = %.3f", k_value);
					send_string(ch);
					break;
				case OP_KI_ROLL:
					k_value = (float) receiveddata / 100;
					sprintf(ch, "\n\rRoll Ki value = %.3f", k_value);
					send_string(ch);
					break;
				case OP_KD_ROLL:
					k_value = (float) receiveddata / 100;
					sprintf(ch, "\n\rRoll Kd value = %.3f", k_value);
					send_string(ch);
					break;
				case OP_KP_YAW:
					k_value = (float) receiveddata / 100;
					sprintf(ch, "\n\rYaw Kp value = %.3f", k_value);
					send_string(ch);
					break;
				case OP_KI_YAW:
					k_value = (float) receiveddata / 100;
					sprintf(ch, "\n\rYaw Ki value = %.3f", k_value);
					send_string(ch);
					break;
				case OP_KD_YAW:
					k_value = (float) receiveddata / 100;
					sprintf(ch, "\n\rYaw Kd value = %.3f", k_value);
					send_string(ch);
					break;
				case OP_KP_PITCH:
					k_value = (float) receiveddata / 100;
					sprintf(ch, "\n\rPitch Kp value = %.3f", k_value);
					send_string(ch);
					break;
				case OP_KI_PITCH:
					k_value = (float) receiveddata / 100;
					sprintf(ch, "\n\rPitch Ki value = %.3f", k_value);
					send_string(ch);
					break;
				case OP_KD_PITCH:
					k_value = (float) receiveddata / 100;
					sprintf(ch, "\n\rPitch Kd value = %.3f", k_value);
					send_string(ch);
					break;
				}
			}
#endif // ENABLE_UI

		}
		if (i >= 150 && !PIDmode)
		{
			send_string("Sending telemetry");
			Send_data(OP_THRUST, receiveddata);
			_delay_ms(2);
			Send_data(OP_THRUST, receiveddata);
			_delay_ms(2);
			rfm12_tick();
			i = 0;
		}
	}
	//sprintf(ch, "\n\rI = %u", i);
	//send_string(ch);
	while (1)
	{

	}
}