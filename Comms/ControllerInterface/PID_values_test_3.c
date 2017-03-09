// Code for entering PID constants values on putty and then
// step 1: get them ton display on screen 
// step 2: Once step 1 is done get it to display then figure out how to convert them to 8 bit values that show a resolution between 0 and 1
// Note : compared to PID_values_test_2 we need to add ask user to input pid values for yaw pitch and roll
// and display them
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include "debug.h"
#define BAUD 57600//9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR
#define CHAR_MAX 6
//********** from servo.c

#define PWM_DUTY_MAX 240
#define PWM_DUTY_MIN 0
#define PWM_PRESCALER 8UL
#define PWM_FREQUENCY 50
#define PWM_OFFSET 0
//**********

/*Includes usart.h header file which defines different functions for USART. USART header file version is 1.1*/
void adc_init()//[1]
{
	
	// In ADCSRA Enable ADC (set ADEN) and prescaler of 64
	ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
}
void init_pwm(void)
{
   	/* TIMER 2 */
    DDRD |= _BV(PD6); /* PWM out */
    DDRD |= _BV(PD7); /* inv. PWM out */
    

    TCCR2A = _BV(WGM20) | /* fast PWM/MAX */
	     _BV(COM2A1); /* A output */
    TCCR2B = _BV(CS21)  | 
             _BV(CS22);   /* 1/256 prescaling */
}
uint16_t adc_read(int n)//[1]
{
	ADMUX = n;// represents PA2
	// start conversion
	ADCSRA |= _BV(ADSC);
	// wait for conversion to complete
	//while(!(ADCSRA & _BV(ADIF))){};
	while(ADCSRA & _BV(ADSC));
	ADC = (ADCH << 8) | ADCL;// [1]
	return ADC;
}
void pwm_duty(uint8_t duty)// from servo.c
{
	duty = duty > PWM_DUTY_MAX ? PWM_DUTY_MAX : duty;
	duty = duty < PWM_DUTY_MIN ? PWM_DUTY_MIN : duty;
    	//printf("\nPWM=%3u  ==>  ", duty);  
	OCR2A = duty;
}
void init_uart1()// initialize UART
{
	 //1. set the baud rate, lets configure to 9600;
	// set the baud rate registers Ref: [1],[2]
	UBRR0H = BAUDRATE >> 8;// UBRRnH is 8 bits left
	UBRR0L = BAUDRATE;

	 //2. setting up data packet: 8 bits ,no parity 1 stop bit
		// setting 8 bits got to UCSCR register Ref:[3], pg 185 of data sheet

	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01); // 8 bits, USBS1 = 0 for 1 stop bit

		// note: havnt set up the stop bit in Ref [2] slides
	// 3. from Ref[2] we now enable Transmission and receive n UCSRnB register
	UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);// enable rx interrupt

}
// transmit data function to transmit to the screen 
void uart_transmit( char data)
{
	while(!( UCSR0A &  _BV(UDRE0) ) ); //  data register enable bit is 1 if tx buffer is empy
	// if its 1 we load data onto UDR- Uart Data Register(buffer)
	UDR0 = data;
}
// reveive data function to receive values entered on screen
// char uart_receive()
// {
// 	if (!(UCSR0A &  _BV(RXC0) ))// if there is unread data in the receive whihc needs to be read
// 		return UDR0;
// 	else
// 		return NULL;
// }
void send_string(char *str)
{
	int i;
	for( i = 0; str[i]; i++) uart_transmit(str[i]);
}//***************
char k[CHAR_MAX];
volatile uint8_t counter = 0;// itss uint8_t for size saving
volatile uint8_t pid_enter_check = 0; // used to ensure number(eg: 10.230) is not passed to k[] when asked to enter p or i or d
volatile uint8_t tyh_enter_check = 0;
volatile char pid, tyh, pid_copy, tyh_copy;
char buff[10];
ISR(USART0_RX_vect)
{
	char temp[CHAR_MAX];
	temp[counter] = UDR0;
	// if user entrs t/y or h when asked to enter p or i or d then skip this if statement
	// and go to the 5th if statement 
	//          |
	//          V
	if ( (pid_enter_check == 0) && (temp[counter] == 'r' || temp[counter] == 'y' || temp[counter] == 'h'))
	{
		tyh_enter_check = 0;// disbale checker as the user has entered thrust or yaw or pitch as requested
		tyh = temp[counter];
		uart_transmit(tyh);
	}
	else if ((tyh_enter_check == 1) && !(temp[counter] == 'r' || temp[counter] == 'y' || temp[counter] == 'h'))//(temp[counter] > 47 || temp[counter] < 58)
	{
		uart_transmit(temp[counter]);
		send_string("\n\r Roll(r) or Yaw(y) or Pitch(h) pls!: ");		
		counter = 0;// ensure buffer isnt filled with entered numbers by re-setting this counter
	}
	else if (temp[counter] == 'p' || temp[counter] == 'i' || temp[counter] == 'd')
	{
		pid_enter_check = 0;// disbale checker as the user has entered p or i or d as requested
		pid = temp[counter];
		uart_transmit(pid);// print it to see what we enter
	}
	else if (temp[counter] == 'x')
	{
		counter = 0;//reset the counter to zero so that u can start re-writing to the k[] array
		send_string("\n\r Re-enter roll(r)/yaw(y)/pitch(h) : ");
		// reset the entry checkers to reset the entry of the details
		tyh_enter_check = 1;// to get to the 2nd if condition

	}
	else if ((pid_enter_check == 1) && !(temp[counter] == 'p' || temp[counter] == 'i' || temp[counter] == 'd'))//(temp[counter] > 47 || temp[counter] < 58)*/ )// incase u entered value when asked to enter p or i or d
	{
		uart_transmit(temp[counter]);
		send_string("\n\r Pls p or i or d!: ");
		counter = 0;// ensure buffer isnt filled with entered numbers by re-setting this counter
	}
	else // if numbers or dots were entered
	{
		k[counter] = temp[counter];// can store digits or dots and incremet counter
		uart_transmit(k[counter]);// print it to see what we enter
		counter++;// incremet to get next digit 
	}
	//  ASCII : 47 < x < 57 corresponds to integers 0-9 and x = 46 which is a 'dot' which we EXCLUDE in this condition to DEAL with the "other characters" apart fron 'p' or 'i' or 'd'
	// else if ( (temp[counter] < 47 || temp[counter] > 57) && (temp[counter] != 46) )// if what you entered are letters like 'l' or 'z' etc. when asked to enter p or i or d and 
	// {
	// 	send_string("\n\r Please enter p or i or d: "); // ask to re enter p or i or d
	// 	counter = 0;// BUT if numbers were ented when asked to enter p or i or d 
	// 	// re-write the the k[] buffer to get rid of those numbers entered
	// }
	
}

int main()
{
	init_uart1();
	init_pwm();
	adc_init();
	//init_debug_uart0();
	sei();// enable global interrupt
	char ch[60];
	float f, f_temp;
	uint16_t ten_bit;
	send_string("\n\rEnter roll(r)/yaw(y)/pitch(h): ");
	tyh_enter_check = 1;
	uint16_t adc_value;
	uint8_t pwm_value;

	while(1)
	{
		
		//receive_string(ki);
		//ki[i] = uart_receive();
		adc_value = adc_read(0);
		pwm_value = (uint8_t) (adc_value/4) + PWM_OFFSET;	
		pwm_duty(pwm_value);
		// we are having a servio as way of checking if this code doesnt block the flow of this while loop significantly
		if (tyh == 'r' || tyh == 'y' || tyh == 'h')
		{
			send_string("\n\r Enter a K type (p or i or d): ");
			pid_enter_check = 1;// always set this variable to 1 when asked to enter p or i or d
			tyh_copy = tyh;// we need a copy of tyh to display what we have to the user
			tyh = NULL;// definitely clear this as this condition will keep getting satisfied!

		}

		else if (pid == 'p' || pid == 'i' || pid == 'd')
			{
				send_string("\n\r Enter k value of this type: ");
				pid_copy = pid;
				pid = NULL;// definitely clear this as this condition will keep getting satisfied!
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
			sprintf(ch,"\n\r 10-bit dec value for k type: %c of %c: %d",pid_copy,tyh_copy,ten_bit);
			send_string(ch);
			// transmit transmit the data for pid_copy,tyh_copy and ten_bi

			send_string("\n\rEnter Thrust(t)/yaw(y)/pitch(h): ");
			tyh_enter_check = 1;
		}
		else
			continue;
	     
	
	}
}
