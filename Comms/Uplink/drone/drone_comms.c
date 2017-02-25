#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "rfm12.h"
#include "drone_comms.h"

typedef enum { Thrust = 0, Roll, Yaw, SecCheck } Datatype;

int main(void)
{	
	// Initialise rfm12 and interrupts
	rfm12_init();
	sei();

	uint8_t receiveddata = 0;

	while (1)
	{
		rfm12_tick();
		
		// Wait for data to be fully received
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{	
			Datatype receivedtype = (Datatype)rfm12_rx_type();
			switch (receivedtype)
			{
			case Thrust:
				break;
			case Roll:
				break;
			case Yaw:
				break;
			case SecCheck:
				break;
			}
		}
	}
}

// Send relevant data to the control Il Matto
/*
void Send_controls()
{

}
*/