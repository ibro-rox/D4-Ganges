// This code is now going to involve taking the copy of basestation_comms.c and adding the adc bits from base_station_controller.c
//  date: 1/3/17
// Arthur: Mohammed Ibrahim and Joel
// D4-Ganges!!!!!
// This code is a aprt of test 1 I mentioned in my pink book
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

	char ch[30];
	while (1)
	{
		rfm12_tick();
		#if UPLINK_TEST
			Send_data(OP_ROLL, testdata);
			sprintf(ch, "\n\rTestdata = %d", testdata);
			send_string(ch);
			testdata++;
			if (testdata == 1024) break;
			_delay_ms(500);
		#endif
		#if ENABLE_CONTROLS
			thrust = adc_read(PIN_THRUST);
			sprintf(ch,"\n\r thrust = %d",thrust);// To see what we are transmitting
			send_string(ch);
			_delay_ms(500);
			Send_data(OP_THRUST, thrust);

			yaw = adc_read(PIN_YAW);
			sprintf(ch,"\n\r Yaw = %d",yaw);// To see what we are transmitting
			send_string(ch);
			_delay_ms(500);
			Send_data(OP_YAW, yaw);

			pitch = adc_read(PIN_PITCH);
			sprintf(ch,"\n\r Pitch = %d",pitch);// To see what we are transmitting
			send_string(ch);
			_delay_ms(500);
			Send_data(OP_PITCH, pitch);

			roll = adc_read(PIN_ROLL);
			sprintf(ch,"\n\r Roll = %d",roll);// To see what we are transmitting
			send_string(ch);
			_delay_ms(500);
			Send_data(OP_ROLL, roll);
		#endif
	}

	// Allows program to stop
	while (1) {};
}

