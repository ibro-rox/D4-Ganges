#include <avr/io.h>
#include <util/delay.h>

#include <avr/interrupt.h>

#include "debug.h"

/* #define BAUD 57600

#ifdef BAUD
#include <util/setbaud.h>
#endif */

volatile uint8_t serial_interrupt_flag = 0;

ISR(INT0_vect)
{
	serial_interrupt_flag = 1; 			// Interrupt sets flag could be replaced
}													// with longer ISR if immediately needed

struct euler_angles {						//Structure containing TYPR
	uint16_t throttle;
	uint16_t yaw;
	uint16_t pitch;
	uint16_t roll;
};

uint8_t is_valid_packet(struct euler_angles *input);
void send_packet(struct euler_angles *input);

int main()
{
	
	init_debug_uart0();
	
	struct euler_angles desired_values;
	
	desired_values.throttle = 256;
	desired_values.yaw = 554;
	desired_values.pitch = 747;
	desired_values.roll = 517;
	
	DDRD |= _BV(PD2);			// Set PD2 to output
	PORTD &= ~_BV(PD2);
	EICRA |= _BV(ISC01) | _BV(ISC00); 	// Set to trigger on rising edge
	EIMSK |= _BV(INT0); 			// Enable interrupt 
	sei();					// Enable global interrupt 
	
	/*
	INSERT REST OF INIT HERE
	*/
	
	while(1)
	{
		if(serial_interrupt_flag)
			send_packet(&desired_values);
		/*
		INSERT REST OF LOOP HERE
		*/		
	}
}

void send_packet(struct euler_angles *input)
{
	if(is_valid_packet(input))
		printf("t%dy%dp%dr%d\n", input->throttle, input->yaw, input->pitch, input->roll);
	else 
		printf("t%dy%dp%dr%d\n", 0, 0, 0, 0);
	
		serial_interrupt_flag = 0;
}

uint8_t is_valid_packet(struct euler_angles *input)
{
	return ((input->throttle <= 1023) && (input->yaw <= 1023) && 
			(input->pitch <= 1023) && (input->roll <= 1023));
}
