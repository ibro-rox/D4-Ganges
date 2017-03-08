// Arthur: Mohammed Ibrahim
// Read 4 potentiometers which are 4 channels : THRUT, AILE, RUDD, ELEV
// Acknowledgement: [1] Had to re-write code from start for the adc functions 
// which were taken from Rhys thomas 
// Potential reason for my code not working : didnt do line 25
// here ADMUX = 2 represents PB3
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "rfm12.h" // for uplink trasceiver 
// #include "rfm12.h" // for downlink transceiver 
uint16_t thrust,yaw,pitch,roll;
//**** 
#define THRUST_TYPE 00 // 0x00 isnt a the actual vallue
#define PITCH_TYPE 01
#define YAW_TYPE 10
#define ROLL_TYPE 11
//****
// initialzie adc
void adc_init()//[1]
{
	

	// In ADCSRA Enable ADC (set ADEN) and prescaler of 64
	ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
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
// transmit data function
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
}//***************
int main()
{
	adc_init();
	uplink_rfm12_init();// initialize rfm12 transceiver
	downlink_rfm12_init();
	sei();// enable the ISR in the rfm12.h
	while(1)// main forver loop
	{
		thrust = adc_read(0);// 10 bit value
		// split the 10 bit to 2 bits()
		// transmit it - rfm12_tx() and rfm_tick()
		// rfm12_tx() - fills the tx buffer and transmits the 8-bit type and 8-bit data
		// rfm_tick() checks if channel is free to send next data packet
		// potential delay for sync 
		yaw = adc_read(1);
		// split the 10 bit to 2 bits()
		// transmit it - rfm12_tx() and rfm_tick()
		// potential delay for sync 
		pitch = adc_read(2);
		// split the 10 bit to 2 bits()
		// transmit it - rfm12_tx() and rfm_tick()
		// potential delay for sync 
		roll = adc_read(3);
		// split the 10 bit to 2 bits()
		// transmit it - rfrm12_tx() and rfm_tick()
		//_delay_ms(100); ptoential delay to worry about ater 
		// this base station code needs to do reception as well in this while loop 
		// functions for down link transceiver

		if (rfm12_rx_status() == STATUS_COMPLETE)// if receiveing data is done, then read the buffer
		{
			uint8_t channel_type = rfm12_rx_type(); // read the 8- bit type
			uint8_t channel_data = rfm12_rx_buffer();// read the 8 - bit data
			// important to clear the receiver buffer
			rfm12_rx_clear();
			// then re-obtain the 10-bit data from the 8-bit packet and 8- bit type
			// joel's code bit
			//look at the data type and identify which channel it is
			char ch[20];
			switch (channel_type)
			{
				case THRUST_TYPE: {
									sprintf(ch,"THRUST = %d",channel_data);
									send_string(ch);
									}
								break;
				case PITCH_TYPE: {
									sprintf(ch,"PITCH = %d",channel_data);
									send_string(ch);
									}
								break;
				case ROLL_TYPE: {
									sprintf(ch,"ROLL = %d",channel_data);
									send_string(ch);
									}
								break;
				case YAW_TYPE: {
									sprintf(ch,"ROLL= %d",channel_data);
									send_string(ch);
									}
								break;
			}
		}
		
	}
}