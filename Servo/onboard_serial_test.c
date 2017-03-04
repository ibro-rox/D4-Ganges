#include <avr/io.h>
#include <util/delay.h>

#include <avr/interrupt.h>

#include "debug.h"

#define BAUD 9600
#include <util/setbaud.h>

volatile uint8_t serial_interrupt_flag = 0;

ISR(INT0_vect)
{
	//printf("Hello from ISR");
	serial_interrupt_flag = 1;
}

struct euler_angles {
	uint16_t throttle;
	uint16_t yaw;
	uint16_t pitch;
	uint16_t roll;
};



void send_packet(struct euler_angles *input);



int main()
{
	
	init_debug_uart0();
	
	struct euler_angles desired_values;
	
	desired_values.throttle = 1;
	desired_values.yaw = 2;
	desired_values.pitch = 3;
	desired_values.roll = 4;
	
	DDRD |= _BV(PD2);
	PORTD &= ~_BV(PD2);
	/* Set to trigger on rising edge */
	EICRA |= _BV(ISC01) | _BV(ISC00);
	/* Enable interrupt */
	EIMSK |= _BV(INT0);
	sei();	
	
	while(1)
	{
		if(serial_interrupt_flag)
			send_packet(&desired_values);
		
		
		
		
		/* for(i=0;i<10;i++)
		{
			printf("%d = 0x%x\n",i,segments[i]);
			_delay_ms(1000);
			
		}	
		 fprintf(stderr,"Count overflow\n\r"); */
	}
}

void send_packet(struct euler_angles *input)
{
	if((input->throttle <= 1023) && (input->yaw <= 1023) && (input->pitch <= 1023) && (input->roll <= 1023))
		printf("T%d Y%d P%d R%d\n", input->throttle, input->yaw, input->pitch, input->roll);
	else 
		printf("Error\n");
	
		serial_interrupt_flag = 0;
}