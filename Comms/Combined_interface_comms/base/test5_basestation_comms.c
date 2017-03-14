// This code is now going to involve taking the copy of basestation_comms.c and adding the adc bits from base_station_controller.c

//  date finished : 8/3/17

// Arthur: Mohammed Ibrahim and Joel

// D4-Ganges!!!!!

// This code can:

//- send adc values and cargo on/off during flight mode (switch is on flight mode)
// - send each k type values for each channel as the user enters them(switch is on PID/ UI mode)
// UI is more robust, can re-enter enter details again (starting again from entering yaw/pitch/roll)
// Extra feature but might potentially be an important feature to increase data rate:
// During flight mode, send adc values only when there is a change in the channels to hopefully improve datarate (ACHIEVED!!!!!!!!!!!!!!!)

// Same as test4 but aiming to add these features :
// Important feature: get inputs from two more swithces and transmit them(ONE OF THEM DONE)
// do Telemetry interpretation ie do case statement to see what the received data represents: - 3 angle values and voltage of the(DONE!!)


#include <avr/io.h>
#include <stdio.h>// for sprintf()
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>

#include "rfm12.h"
#include "basestation_comms.h"

//***************
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
		//send_string("\n\r FLAGGG");
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
	
	
}
//************************************************ from PID_control_test_3

int main(void)

{

	// Initialise rfm12, adc, uart and interrupts

	rfm12_init();

	adc_init();

	init_uart1();

	sei();
	//**************************************
	#if ENABLE_UI
	char ui[60];
	float f, f_temp;
	uint16_t ten_bit;
	send_string("\n\rEnter roll(r)/yaw(y)/pitch(h): ");
	tyh_enter_check = 1;
	//uint16_t adc_value;
	//uint8_t pwm_value;
	#endif
	//******************************************
	#if UPLINK_TEST

		uint16_t testdata;

		testdata = 0;

	#endif



	// ADC channels 
    #if ENABLE_CONTROLS
	uint16_t thrust_previous, yaw_previous, pitch_previous, roll_previous;
	thrust_previous = 0;
	yaw_previous = 0;
	pitch_previous = 0;
	roll_previous = 0;
	uint16_t thrust_present, yaw_present, pitch_present, roll_present;

	// telemetry values
	uint8_t packettype;
	uint16_t data;
	// Button for cargo
	DDRA &= ~_BV(PA6);// enable input
	PORTA |= _BV(PA6);// enable pull ups
	// flight/pid siwtch
	DDRA &= ~_BV(PA7);// enable input
	PORTA |= _BV(PA7);// enable pull ups
	// kill switch
	DDRA &= ~_BV(PA4);// enable input
	PORTA |= _BV(PA4);// enable pull ups


	DDRD |= _BV(PD6);// SETto output
	PORTD |= _BV(PD6);// SET TO HIGH
	// DDRD |= _BV(PD7);// SETto output
	// PORTD |= _BV(PD7);// SET TO HIGH
	// button states
	uint8_t button_present, button_previous;
	uint8_t flight_present, flight_previous;
	uint8_t kill_present, kill_previous;
	flight_previous = 0;
	button_previous = 0;
	kill_previous = 0;
	char ch[40], telemetry[40];
	float angle, voltage;
	#endif
	Send_data(OP_BUTTON, flight_present = (PINA & _BV(PA7))? BTN_FLIGHT_MODE:BTN_PID_MODE);// sending an initial mode to the receiver
	while (1)

	{

		flight_present = (PINA & _BV(PA7))? 1:0;

		if ((flight_present == 1) && (flight_previous == 0))
		{
			Send_data(OP_BUTTON,BTN_FLIGHT_MODE);
			_delay_ms(2);
			Send_data(OP_BUTTON,BTN_FLIGHT_MODE);
			_delay_ms(2);
			rfm12_tick();
		}
		else if ((flight_present == 0) && (flight_previous == 1))
		{
			// we are having a servio as way of checking if this code doesnt block the flow of this while loop significantly
			Send_data(OP_BUTTON,BTN_PID_MODE);
			_delay_ms(2);
			Send_data(OP_BUTTON,BTN_PID_MODE);
			_delay_ms(2);
			rfm12_tick();
			send_string("\n\rEnter roll(r)/yaw(y)/pitch(h): ");
		}
		flight_previous = flight_present;
			// check the rx buffer 

		if (flight_present)
		{

			if (rfm12_rx_status() == STATUS_COMPLETE)
			{
				packettype = rfm12_rx_type();
				data = *rfm12_rx_buffer();

				Retrieve_data(&packettype, &data);
				//sprintf(ch, "\n\rPacket type: %u and Data: %u", packettype, data);
				//send_string(ch);
				rfm12_rx_clear();
				switch(packettype)
				{
					case OP_ANGLE_X : // angle needs to be converted from its decimal value to float
										//angle = ; 
										sprintf(telemetry,"Gyro angle x: %f", angle);
										send_string(telemetry);
									break;
					case OP_ANGLE_Y: // angle needs to be converted from its decimal value to float
										//angle = ; 
										sprintf(telemetry,"Gyro angle y: %f", angle);
										send_string(telemetry);
									break;
					case OP_ANGLE_Z: // angle needs to be converted from its decimal value to float
										//angle = ; 
										sprintf(telemetry,"Gyro angle z: %f", angle);
										send_string(telemetry);
									break;
					case OP_BATTERY_LEVEL:  // angle needs to be converted from its decimal value to float
										voltage = (float)(3.3/1024)*data;// r1 = 100k and r2 is 33k
										voltage = (float)(133/33)*voltage;
										sprintf(telemetry,"\n\rBattery voltage: %f", voltage);
										send_string(telemetry);
									break;
					case OP_IR_SENSOR:
						voltage = to_distance(data);
						sprintf(telemetry, "\n\rIR sensor: %f", voltage);
						send_string(telemetry);
						break;
				}
			}
			//else
				//send_string("\n\r No telemetry :(");

			thrust_present = (kill_present) ? 512 : adc_read(PIN_THRUST);
			if (thrust_present != thrust_previous && thrust_present < 1020)
			{
				Send_data(OP_THRUST, thrust_present);
				_delay_ms(2);
				Send_data(OP_THRUST, thrust_present);
				_delay_ms(2);
				rfm12_tick();// make sure you 'TICK' EVERY TIME YOU SEND_DATA! (IN OUR CASE SEND TWICE)!!!!!!!!!!!!!!!!!!!!!!!!!!
				thrust_previous = thrust_present;
			}

			yaw_present = (kill_present) ? 512 : adc_read(PIN_YAW);
			if (yaw_present != yaw_previous && yaw_present < 1020)
			{
				Send_data(OP_YAW, yaw_present);
				_delay_ms(2);
				Send_data(OP_YAW, yaw_present);
				_delay_ms(2);
				rfm12_tick();// make sure you 'TICK' EVERY TIME YOU SEND_DATA! (IN OUR CASE SEND TWICE)!!!!!!!!!!!!!!!!!!!!!!!!!!
				yaw_previous = yaw_present;
			}

			pitch_present = (kill_present) ? 512 : adc_read(PIN_PITCH);

			if (pitch_present != pitch_previous && pitch_present < 1020)
			{
				Send_data(OP_PITCH, pitch_present);
				_delay_ms(2);
				Send_data(OP_PITCH, pitch_present);
				_delay_ms(2);
				rfm12_tick();// make sure you 'TICK' EVERY TIME YOU SEND_DATA! (IN OUR CASE SEND TWICE)!!!!!!!!!!!!!!!!!!!!!!!!!!
				pitch_previous = pitch_present;
			}

			roll_present = (kill_present) ? 512 : adc_read(PIN_ROLL);

			if (roll_present != roll_previous && roll_present < 1020)
			{
				Send_data(OP_ROLL, roll_present);
				_delay_ms(2);
				Send_data(OP_ROLL, roll_present);
				_delay_ms(2);
				rfm12_tick();// make sure you 'TICK' EVERY TIME YOU SEND_DATA! (IN OUR CASE SEND TWICE)!!!!!!!!!!!!!!!!!!!!!!!!!!
				roll_previous = roll_present;
			}
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

			kill_present = (PINA & _BV(PA4)) ? 1:0;
			if ((kill_present == 1) && (kill_previous == 0))
			{
				send_string("\n\rKill switch ON!");
				Send_data(OP_BUTTON,BTN_POWER_OFF);
				_delay_ms(2);
				Send_data(OP_BUTTON,BTN_POWER_OFF);
				_delay_ms(2);
				rfm12_tick();
			}
			else if ((kill_present == 0) && (kill_previous == 1))
			{
				send_string("\n\rKill switch OFF!");
				Send_data(OP_BUTTON, BTN_POWER_ON);
				_delay_ms(2);
				Send_data(OP_BUTTON, BTN_POWER_ON);
				_delay_ms(2);
				rfm12_tick();
			}
			kill_previous = kill_present;


			// DO THE 2 MORE BUTTON FEATURES


		}
		else if (!flight_present)
		{
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
				sprintf(ui,"\n\r 10-bit dec value for k type: %c of %c: %d",pid_copy,tyh_copy,ten_bit);
				send_string(ui);
				// transmit the data for pid_copy,tyh_copy and ten_bi
				switch(tyh_copy)
				{
					case 'r':   if (pid_copy == 'p')
									{
										Send_data(OP_KP_ROLL,ten_bit);
										_delay_ms(2);
										// Send_data(OP_KP_ROLL,ten_bit);
										// _delay_ms(2);
										rfm12_tick();
									}
								else if (pid_copy == 'i')
								{
									Send_data(OP_KI_ROLL,ten_bit);
									_delay_ms(2);
									// Send_data(OP_KI_ROLL,ten_bit);
									// _delay_ms(2);
									rfm12_tick();
								}
								else if (pid_copy == 'd')
								{
									Send_data(OP_KD_ROLL,ten_bit);
									_delay_ms(2);
									// Send_data(OP_KD_ROLL,ten_bit);
									// _delay_ms(2);
									rfm12_tick();
								}
								break;
					case 'y':   if (pid_copy == 'p')
									{
										Send_data(OP_KP_YAW,ten_bit);
										_delay_ms(2);
										// Send_data(OP_KP_YAW,ten_bit);
										// _delay_ms(2);
										rfm12_tick();
									}
								else if (pid_copy == 'i')
								{
									Send_data(OP_KI_YAW,ten_bit);
									_delay_ms(2);
									// Send_data(OP_KI_YAW,ten_bit);
									// _delay_ms(2);
									rfm12_tick();
								}
								else if (pid_copy == 'd')
								{
									Send_data(OP_KD_YAW,ten_bit);
									_delay_ms(2);
									// Send_data(OP_KD_YAW,ten_bit);
									// _delay_ms(2);
									rfm12_tick();
								}
								break;
					case 'h':   if (pid_copy == 'p')
									{
										Send_data(OP_KP_PITCH,ten_bit);
										_delay_ms(2);
										// Send_data(OP_KP_PITCH,ten_bit);
										// _delay_ms(2);
										rfm12_tick();
									}
								else if (pid_copy == 'i')
								{
									send_string("\n\rFLAG!");
									Send_data(OP_KI_PITCH,ten_bit);
									_delay_ms(2);
									// Send_data(OP_KI_PITCH,ten_bit);
									// _delay_ms(2);
									rfm12_tick();
								}
								else if (pid_copy == 'd')
								{
									Send_data(OP_KD_PITCH,ten_bit);
									_delay_ms(2);
									// Send_data(OP_KD_PITCH,ten_bit);
									// _delay_ms(2);
									rfm12_tick();
								}
								break;
				}
				// clear pid_copy and tyh type sp that they dont transmitted again
				pid_copy = NULL;
				tyh_copy = NULL;
				send_string("\n\rEnter roll(r)/yaw(y)/pitch(h): ");
				tyh_enter_check = 1;
				counter = 0;// to stop receivng these datas event houghtb i havnt entered anything
			}
			else
				continue;
		}
		

	}



	// Allows program to stop

	while (1) {};

}