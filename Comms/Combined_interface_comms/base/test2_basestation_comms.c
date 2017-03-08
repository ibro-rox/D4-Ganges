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

//*****************************************************************************
char k[CHAR_MAX];
volatile uint8_t counter = 0;// itss uint8_t for size saving
volatile uint8_t pid_enter_check = 0; // used to ensure number(eg: 10.230) is not passed to k[] when asked to enter p or i or d
volatile char pid;
char buff[10];
ISR(USART0_RX_vect)
{
	char temp[CHAR_MAX];
	temp[counter] = UDR0;
	if (temp[counter] == 'p' || temp[counter] == 'i' || temp[counter] == 'd')
	{
		pid = temp[counter];
		uart_transmit(pid);// print it to see what we enter
	}
	else if (temp[counter] == 'x')
	{
		counter = 0;//reset the counter to zero so that u can start re-writing to the k[] array
		send_string("\n\r Re-enter K value: ");
	}
	else if ( (pid_enter_check == 1) && (temp[counter] > 47 || temp[counter] < 58) )
	{
		uart_transmit(temp[counter]);
		send_string("\n\r p or i or d pls!: ");
		counter = 0;// ensure buffer isnt filled with these numbers by re-setting this counter
		pid_enter_check = 0;
	}
	//  ASCII : 47 < x < 75 corresponds to integers 0-9 and x = 46 whihc is a 'dot' which we EXCLUDE in this condition to DEAL with the "other characters" apart fron 'p' or 'i' or 'd'
	else if ( (temp[counter] < 47 || temp[counter] > 57) && (temp[counter] != 46) )// if what you entered are letters like 'l' or 'z' etc. when asked to enter p or i or d and 
	{
		send_string("\n\r Please enter p or i or d: "); // ask to re enter p or i or d
		counter = 0;// BUT if numbers were ented when asked to enter p or i or d 
		// re-write the the k[] buffer to get rid of those numbers entered
	}
	else // if numbers or dots were entered
		{
			k[counter] = temp[counter];// can store digits or dots and incremet counter
			uart_transmit(k[counter]);// print it to see what we enter
			counter++;// incremet to get next digit 
		}
}
//*****************************************************************************

int main(void)
{
	// Initialise rfm12, adc, uart and interrupts
	rfm12_init();
	adc_init();
	init_uart1();
	sei();

	// Send test data
	uint16_t testdata;
	//
	uint16_t ten_bit;
	testdata = 0;
	uint16_t thrust, yaw, pitch, roll;// start with one channel transmisssion sinc eat this time we have one pot
	float f_temp, f;
	char ch[30];
	while (1)
	{
		rfm12_tick();
		#if ENABLE_CONTROLS 
		thrust = adc_read(0);
		sprintf(ch,"\n\r thrust = %d",thrust);// To see what we are transmitting
		send_string(ch);
		_delay_ms(10);
		Send_data(OP_THRUST, thrust);
		yaw = adc_read(1);
		sprintf(ch,"\n\r Yaw = %d",yaw);// To see what we are transmitting
		send_string(ch);
		_delay_ms(10);
		Send_data(OP_YAW, yaw);
		pitch = adc_read(0);
		sprintf(ch,"\n\r Pitch = %d",pitch);// To see what we are transmitting
		send_string(ch);
		_delay_ms(10);
		Send_data(OP_PITCH, pitch);
		roll = adc_read(0);
		sprintf(ch,"\n\r Roll = %d",roll);// To see what we are transmitting
		send_string(ch);
		_delay_ms(10);
		Send_data(OP_ROLL, roll);
		#endif
		// we are having a servio as way of checking if this code doesnt block the flow of this while loop significantly
		if (pid == 'p' || pid == 'i' || pid == 'd')
			{
				send_string("\n\r Enter k value (press x to re-enter): ");
				// BEFORE NULLING IT transmit it!!!!!!!!!!!!!!!
				//pid = NULL;
			}

		else if (counter == CHAR_MAX)// char bufer is ready to transmit
		{
			counter = 0;// back to zero
			f = atof(k);// convert it to float
			f_temp = f*100; // 
			if (f_temp > 1023)// check if number not withing range
			{
				send_string("\n\r Error: k value is not in range!");
				send_string("\n\r Enter a K type (p or i or d): ");
				continue;
			}
			ten_bit = (uint16_t) (f_temp+0.5);
			sprintf(ch,"\n\r 10-bit dec value: %d",ten_bit);
			send_string(ch);
			// transmit its 
				switch (pid)
				{
					case 'p' : Send_data(OP_KP, ten_bit);
								break;
					case 'i' : Send_data(OP_KI, ten_bit);
								break;
					case 'd' : Send_data(OP_KD,ten_bit);
								break;
				}
			//***********
			send_string("\n\r Enter a K type (p or i or d): ");
			pid_enter_check = 1;// always set this variable to 1 when asked to enter p or i or d
		}
		else
			continue;
	}
	
}

