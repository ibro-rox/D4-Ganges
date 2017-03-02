// Arthur: Mohammed Ibrahim
// Code function: to read the pwm signals from the 4 channels : THRUT, AILE, RUDD, ELEV
// This code uses pin change interrupts (one pin on each port only) to read pwm
// Triggers at rising edge of pwm: read data from timer at that instance
// Trigger at falling edge of pwm: read data from timer at that instance
// Get the difference in values at those instance (plus some maths) to get the corresponding duty cycle
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
// global variable
volatile uint16_t thrust_count = 0;

// ISR**********

// ISR(PCINTB_vect)
// {

// }
// ISR(PCINTC_vect)
// {

// }
// ISR(PCINTD_vect)
// {

// }
//************UART SECTION FOR PRINTING DUTY CYCLE DATA***
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

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
	UCSR0B = _BV(TXEN0) | _BV(RXEN0);

}
void uart_transmit( char data)
{
	while(!( UCSR0A &  _BV(UDRE0) ) ); //  data register enable bit is 1 if tx buffer is empy
	// if its 1 we load data onto UDR- Uart Data Register(buffer)
	UDR0 = data;
}
void send_string(char *str)
{
	int i;
	for( i = 0; str[i]; i++) uart_transmit(str[i]);
}
//**************
ISR(PCINT1_vect)
{
	// read from the pin to see if its high or low to indicate rising edge
	//send_string("Hello ISR\n");// IT WORKS!
	if (PINB & _BV(2))// read 16-timer value
		TCNT1 = 0;			// initialize timer to zero [2]
	else                    // and keep counting up 
	 {thrust_count = TCNT1;// record value at instance of falling edge
	char ch[20];
	sprintf(ch, "%d",(int)thrust_count);// conver to string
		send_string(ch);}

}
//***********
int main()
{
	// Enable the following ports as interupts
	// Each port A-D is enabled as we are using one pin for each
	// to read a channel (we are not going for 4 pins on one port[1])
	PCICR |= _BV(PCIE1); /*| _BV(PCIEB) | _BV(PCIEC) | _BV(PCIED)*/;
	// Enable pin 1 on each port as input
	DDRB = 0x00;
	PORTB = 0XFF;// pull up enable on all pins
	// DDRB &= ~_BV(1);
	// DDRC &= ~_BV(1);
	// DDRD &= ~_BV(1);
	// Enable pin 1 on each port as pin channge and mask out the rest of the pins
	PCMSK1 = 0XFF;
	// PCMSKB &= _BV(1);
	// PCMSKC &= _BV(1);
	// PCMSKD &= _BV(1);
	// set timer output
	// we are not using a Ooutput compare reigtser so no need to set any 
	// Compare output mode
	// tehn set pwm mode to fast pwm
	TCCR1A |= _BV(WGM12) | _BV(WGM11);// fast PWM, TOP=ICR1
	// set the prescaler to the highest: Clk_i/o/1024
	TCCR1B |= _BV(CS12) | _BV(CS10);	
	// initialize UART

	// enable global interrupts
	sei();
	int thrust_duty = 0;// corresponding duty
	float Tp = 0.0213;// duration of a pulse
	float T_count = 1024/F_CPU;
    init_uart1();
		 char ch[20];
	 send_string("Hello again\n");
    while(1)// loop keeps running while we wait for pin changes
    {
    	
    		  if (thrust_count)
    		  {
    		  	thrust_duty = ((thrust_count*T_count)/Tp)*100;// need to figure out the conversion
				sprintf(ch, "%d",(int)thrust_duty);// conver to string
				send_string(ch);

    		  }
    
    }
}
//Acknowledgemet:
// [1] From Ben's idea to mask all pins on a port except the pin
// you will connect to the channel
//[2] https://www.rcgroups.com/forums/showthread.php?1088986-Reading-PWM-signal