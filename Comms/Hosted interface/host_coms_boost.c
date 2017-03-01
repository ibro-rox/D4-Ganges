
/*****************************************************************

file: host_coms_boost.c

Sample host program to demonstrate communication with IlMatto.
Use together with embedded_coms_boost.c running on IlMatto.


Author: Klaus-Peter Zauner, 2014
Licence: GPL version 2

Based on the code made available 
by Teunis van Beelen under GPL2 at:

   http://www.teuniz.net/RS-232/
and
   http://www.teuniz.net/Timer_code/

To compile on Linux:
  gcc host_coms_boost.c timer.c rs232_d1.c -Wall -s -o2 -o hostcoms 

To compile on Windows:
  gcc host_coms_boost.c timer.c rs232_d1.c -Wall -s -o2 -o host.exe

*****************************************************************/

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232_d1.h"
#include "timer.h"


#define TIMER_MS 300
#define KEY_IN_BUFLEN   256


void print_serial(void);
char *get_line (char *s, size_t n, FILE *f);
char *p;



#define SER_IN_BUFLEN   4096
int cport_nr=6; /* /dev/ttyUSB0 */
unsigned char serbuf[SER_IN_BUFLEN];


#define CMD_BUFFSIZE 50
void send_command(const char *cmd, int val);



int main()
{
	int bdrate = 9600;       /* 9600 baud */

	char line[KEY_IN_BUFLEN];


	if (OpenComport(cport_nr, bdrate))
	{
		printf("Can not open comport\n");
		fflush(stdout);
		return(1);
	}


	/* if(start_timer(TIMER_MS, &print_serial)) */
	/* { */
	/*   printf("\n timer error\n"); */
	/*   fflush (stdout); */
	/*   return(1); */
	/* } */

	printf("\nEnter 'start' to read from serial port.\n");


	for (;;)
	{
		if (!get_line(line, KEY_IN_BUFLEN, stdin)) continue;

		printf("Command: \"%s\" --> ", line);

		if (!strcmp("quit", line)) {
			printf("Bye!\n");
			break;
		}
		else if (!strcmp("stop", line)) {
			printf("Use 'start' to restart serial input.\r\n");
			stop_timer();
		} else if (!strcmp("start", line)) {
			if (start_timer(TIMER_MS, &print_serial)) {
				printf("timer error\n");
			} else {
				printf("Use 'stop' to end serial input.\r\n");
			} 
		}
		else if (!strcmp("start", line)) {
			printf("Commands:\r\n");
			printf("\tType ki, kp or kd with a float.");
		} else {
			printf("Invalid command, type help for a list of commands \n");
		}
		

		stop_timer();
		return(0);
	}
}


void send_command(const char *cmd, int val) 
{
    char cmdstring[CMD_BUFFSIZE];
    
    snprintf(cmdstring,CMD_BUFFSIZE, "%s %u\r\n", cmd, val);
    cprintf(cport_nr, cmdstring );	    

}



void print_serial(void)
{
    int i, n;

    n = PollComport(cport_nr, serbuf, SER_IN_BUFLEN-1);

    if(n > 0)
    {
		serbuf[n] = 0;   /* always put a "null" at the end of a string! */
    
		for(i=0; i < n; i++)
		{
			if(serbuf[i] < 32 && serbuf[i] != '\n')  /* replace control-codes by dots */
			{
				serbuf[i] = '.';
			}
		}
    
		/* printf("received %i bytes: %s\n", n, (char *)serbuf); */
		printf("%s\n", (char *)serbuf);
		fflush (stdout);
    }

    /* printf("received %i bytes\n", n); */
    
}



/* Read a line from stdin 

   Thomas Wolf at
   http://home.datacomm.ch/t_wolf/tw/c/getting_input.html

 */
char *get_line (char *s, size_t n, FILE *f)
{
	char *p = fgets (s, n, f);

	if (p != NULL) {
		size_t last = strlen (s) - 1;

		if (s[last] == '\n') s[last] = '\0'; 
	}
	return p;
}




