// This code is now going to involve taking the copy of basestation_comms.c and adding the adc bits from base_station_controller.c

//  date: 1/3/17

// Arthur: Mohammed Ibrahim and Joel

// D4-Ganges!!!!!

// This code is a aprt of test 3 I mentioned in my pink book to do downlink every second while doing uplink

#include <avr/io.h>

#include <stdio.h>// for sprintf()

#include <avr/interrupt.h>

#include <util/delay.h>

#include <math.h>



#include "rfm12.h"

#include "basestation_comms.h"



int main(void)

{

	// Initialise rfm12, adc, uart and interrupts

	rfm12_init();

	adc_init();

	init_uart1();

	sei();





	#if UPLINK_TEST

		uint16_t testdata;

		testdata = 0;

	#endif



	// ADC channels 

	uint16_t thrust, yaw, pitch, roll;

	// telemetry values
	uint8_t packettype;
	uint16_t data;
	// Button for cargo
	DDRA &= ~_BV(PA6);// enable input
	PORTA |= _BV(PA6);// enable pull ups
	DDRD |= _BV(PD6);// SETto output
	PORTD |= _BV(PD6);// SET TO HIGH
	// button states
	uint8_t button_present, button_previous;
	button_previous = 0;

	char ch[40];

	while (1)

	{

		//rfm12_tick();

		#if UPLINK_TEST

			Send_data(OP_ROLL, testdata);
			//sprintf(ch, "\n\rTestdata = %d", testdata);

			//send_string(ch);

			testdata++;

			if (testdata == 1023) break;

			_delay_ms(2);
			Send_data(OP_ROLL, testdata);
			_delay_ms(2);

		#endif

		#if ENABLE_CONTROLS

			// check the rx buffer 
			if (rfm12_rx_status() == STATUS_COMPLETE)
			{
				packettype = rfm12_rx_type();
				data = *rfm12_rx_buffer();

				Retrieve_data(&packettype, &data);
				sprintf(ch, "\n\rPacket type: %u and Data: %u", packettype, data);
				send_string(ch);
				rfm12_rx_clear();
			}
			//else
				//send_string("\n\r No telemetry :(");

			thrust = adc_read(PIN_THRUST);
			
			Send_data(OP_THRUST, thrust);
			_delay_ms(2);
			Send_data(OP_THRUST, thrust);
			_delay_ms(2);
			rfm12_tick();// make sure you 'TICK' EVERY TIME YOU SEND_DATA! (IN OUR CASE SEND TWICE)!!!!!!!!!!!!!!!!!!!!!!!!!!

			yaw = adc_read(PIN_YAW);


			Send_data(OP_YAW, yaw);
			_delay_ms(2);
			Send_data(OP_YAW, yaw);
			_delay_ms(2);
			rfm12_tick();// make sure you 'TICK' EVERY TIME YOU SEND_DATA! (IN OUR CASE SEND TWICE)!!!!!!!!!!!!!!!!!!!!!!!!!!


			pitch = adc_read(PIN_PITCH);


			Send_data(OP_PITCH, pitch);
			_delay_ms(2);
			Send_data(OP_PITCH, pitch);
			_delay_ms(2);
			rfm12_tick();// make sure you 'TICK' EVERY TIME YOU SEND_DATA! (IN OUR CASE SEND TWICE)!!!!!!!!!!!!!!!!!!!!!!!!!!

			roll = adc_read(PIN_ROLL);


			Send_data(OP_ROLL, roll);
			_delay_ms(2);
			Send_data(OP_ROLL, roll);
			_delay_ms(2);
			rfm12_tick();// make sure you 'TICK' EVERY TIME YOU SEND_DATA! (IN OUR CASE SEND TWICE)!!!!!!!!!!!!!!!!!!!!!!!!!!

			button_present = (PINA & _BV(PA6)) ? 1:0;
			//sprintf(ch,"\n\rbutton_present = %u", button_present);
			//send_string(ch);
			if ((button_present == 1) && (button_previous == 0))
			{
				send_string("\n\rCargo hook up!");
				Send_data(OP_BUTTON, BTN_CARGO_HOOK_UP);
				_delay_ms(2);
				Send_data(OP_BUTTON, BTN_CARGO_HOOK_UP);
				_delay_ms(2);
				rfm12_tick();

			}
			else if ((button_present == 0) && (button_previous == 1))
			{
				send_string("\n\rCargo hook down!");
				Send_data(OP_BUTTON, BTN_CARGO_HOOK_DOWN);
				_delay_ms(2);
				Send_data(OP_BUTTON, BTN_CARGO_HOOK_DOWN);
				_delay_ms(2);
				rfm12_tick();
			}
			button_previous = button_present;
		#endif

	}



	// Allows program to stop

	while (1) {};

}