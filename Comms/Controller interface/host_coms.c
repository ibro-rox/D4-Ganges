
/*****************************************************************

file: host_comsNT_boost.c

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
  gcc host_comsNT_boost.c rs232_d1.c -Wall -s -o2 -o hostcoms

To compile on Windows:
  gcc host_comsNT_boost.c rs232_d1.c -Wall -s -o2 -o host.exe

*****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232_d1.h"


#define DELAY_US 100000
#define KEY_IN_BUFLEN   256


void print_serial(void);
char *get_line (char *s, size_t n, FILE *f);
char *p;

#define SER_IN_BUFLEN   4096
int cport_nr=16; /* /dev/ttyUSB0 */
unsigned char serbuf[SER_IN_BUFLEN];


#define CMD_BUFFSIZE 50
void send_command(const char *cmd, int val);



int main()
{
  int bdrate=9600;       /* 9600 baud */
  char line[KEY_IN_BUFLEN];
  float fvoltage;
  int ivoltage;
  int command = 0;
  double fkp;
  int ikp;
  double fki;
  int iki;
  double fkd;
  int ikd;
  if(OpenComport(cport_nr, bdrate))  
  {
    printf("Can not open comport\n");
    fflush (stdout);
    return(1);
  }
  for(;;){
    if(IsCTSEnabled(cport_nr) == 1){ //function from rs232_d1.c checks to see if comport is open
      printf( "Comms dowm!\n" );
      break;
    }
    printf("\nEnter 'quit', 'read, 'setv', 'setkp', 'setki' or 'setkd'\n"); //options 
    print_serial(); 
    if (!get_line(line, KEY_IN_BUFLEN, stdin)){   //gets line typed in
      print_serial();  
      continue;
    } 
    printf( "Command: \"%s\" --> ", line ); //quit program
    if( !strcmp( "quit", line )) {
      printf( "Bye!\n" );
      break;
    } 

    else if( !strcmp( "read", line )) { //send command 1 to read data 
      command = 1;
      snprintf(line,CMD_BUFFSIZE, "%u\n", command);
      cprintf(cport_nr, line);
      print_serial();     
    } 

    else if( !strcmp( "setv", line )) { //send command 2 to set voltage 
      printf( "Enter a number between 2 and 12\n" );
      command = 2;
      snprintf(line,CMD_BUFFSIZE, "%u\n", command);
      cprintf(cport_nr, line);

      get_line(line, KEY_IN_BUFLEN, stdin); 
      fvoltage = atof(line); //converts to float
      if(fvoltage < 1 || fvoltage > 12){ //checks range
        printf( "Number out of range\n" );
        continue;
      }
      ivoltage = (int)(fvoltage*1000); //converts it to sendable data 
      snprintf(line,CMD_BUFFSIZE, "%u\n", ivoltage);
      printf( "%s\r\n",line );
      cprintf(cport_nr, line);  
    }

    else if( !strcmp( "setkp", line )) { //send command 3 to set kp 
      printf( "Enter a number between 0.1 and 0.0001\n" );
      command = 3;
      snprintf(line,CMD_BUFFSIZE, "%u\n", command);
      cprintf(cport_nr, line);


      get_line(line, KEY_IN_BUFLEN, stdin); 
      fkp = atof(line);
      if(fkp < 0.0001 || fkp > 0.1){ //checks range
        printf( "Number out of range\n" );
        continue;
      }
      ikp = (int)(fkp*100000);
      snprintf(line,CMD_BUFFSIZE, "%u\n", ikp);
      printf( "%s",line );
      cprintf(cport_nr, line); 
    }

    else if( !strcmp( "setki", line )) { //send command 4 to set ki 
      printf( "Enter a number between 0.1 and 0.0001\n" );
      command = 4;
      snprintf(line,CMD_BUFFSIZE, "%u\n", command);
      cprintf(cport_nr, line);


      get_line(line, KEY_IN_BUFLEN, stdin); 
      fki = atof(line);
      if(fki < 0.0001 || fki > 0.1){ //checks range
        printf( "Number out of range\n" );
        continue;
      }
      iki = (int)(fki*100000);
      snprintf(line,CMD_BUFFSIZE, "%u\n", iki);
      printf( "%s",line );
      cprintf(cport_nr, line); 
    }

    else if( !strcmp( "setkd", line )) {//send command 5 to set kd 
      printf( "Enter a number between 0.1 and 0.0001\n" );
      command = 5;
      snprintf(line,CMD_BUFFSIZE, "%u\n", command);
      cprintf(cport_nr, line);


      get_line(line, KEY_IN_BUFLEN, stdin); 
      fkd = atof(line);
      if(fkd < 0.0001 || fkd > 0.1){ //checks range
        printf( "Number out of range\n" );
        continue;
      }
      ikd = (int)(fkd*100000);
      snprintf(line,CMD_BUFFSIZE, "%u\n", ikd);
      printf( "%s",line );
      cprintf(cport_nr, line); 
    }
    else{
      printf( "how?\n");
    }  
  }
  return(0);
}

void send_command(const char *cmd, int val) 
{
    char cmdstring[CMD_BUFFSIZE];
    
    snprintf(cmdstring,CMD_BUFFSIZE, "%s %u\n", cmd, val);
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
      //printf("%d",n);
      printf("%s", (char *)serbuf);
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




