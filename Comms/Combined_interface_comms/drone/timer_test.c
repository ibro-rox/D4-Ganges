#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "rfm12.h"
#include "drone_comms.h"

void init_timer()
{
	TCCR1B |= _BV(CS12) | _BV(CS10);
	TCNT1 = 0;
}

int main(void)
{
	init_uart1();
	init_timer();

	while (1)
	{
		if (TCNT1 == 11719)
		{
			send_string("Timer at 1s");
			TCNT1 = 0;
		}
	}
}