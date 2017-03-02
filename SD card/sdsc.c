
#include "sdsc.h"

unsigned char sdsc_command ( unsigned char cmd, unsigned long arg )
{
	unsigned char r1;

	sdsc -> spi_tx ( cmd | 0x40 ); 
	sdsc -> spi_tx ( arg >> 24 );
	sdsc -> spi_tx ( arg >> 16 );
	sdsc -> spi_tx ( arg >> 8 );
	sdsc -> spi_tx ( arg );

	if ( cmd == SEND_IF_COND )
	  sdsc -> spi_tx ( 0x87 );     
	else 
	  sdsc -> spi_tx ( 0x95 ); 

	while ( ( r1 = sdsc -> spi_rx () ) == 0xff ); //wait response

	return r1; 
}

void sdsc_init ( void )
{
	sdsc -> spi_init ();

	sdsc -> sdsc_assert ();
	while ( 0x01 != sdsc_command ( GO_IDLE_STATE, 0 ) );
	while ( 0x01 != sdsc_command ( SEND_IF_COND, 0x000001AA ) );
	while ( sdsc_command ( APP_CMD, 0 ) && sdsc_command ( SD_SEND_OP_COND, 0x40000000 ) );
	while ( 0x00 != sdsc_command ( READ_OCR, 0 ) );
	sdsc -> sdsc_deassert ();
	
	sdsc -> spi_init_2x ();
}

void read_single_block ( unsigned long block_address, unsigned char *buf )
{
	int i;

	block_address = block_address << 9;

	sdsc -> sdsc_assert ();
	while ( 0x00 != sdsc_command ( READ_SINGLE_BLOCK, block_address ) );
	while ( 0xfe != sdsc -> spi_rx () );
	for ( i = 0; i < 512; i ++ )
  		buf [ i ] = sdsc -> spi_rx ();
	sdsc -> spi_rx (); 
	sdsc -> spi_rx ();
	sdsc -> sdsc_deassert ();;
}

unsigned char write_single_block ( unsigned long block_address, unsigned char *buf )
{
	int i;
	unsigned char r1;

	block_address = block_address << 9;

	sdsc -> sdsc_assert ();

	while ( 0x00 != sdsc_command ( WRITE_SINGLE_BLOCK, block_address ) );
	sdsc -> spi_tx ( 0xfe );     
	for ( i=0; i < 512; i++ )    
  		sdsc -> spi_tx ( buf [ i ] );
	sdsc -> spi_tx ( 0xff );     
	sdsc -> spi_tx ( 0xff );

	r1 = sdsc -> spi_rx ();

	if( ( r1 & 0x1f) != 0x05 )  	   //r1 = 0bXXX0AAA1 ; AAA='010' - data accepted
	{                                  //AAA='101'-data rejected due to CRC error
  		sdsc -> sdsc_deassert ();;            	   //AAA='110'-data rejected due to write error
  		return r1;
	}else;

	while ( ! sdsc -> spi_rx () ); 				//wait till writing completed

	sdsc -> sdsc_deassert ();;

	sdsc -> spi_tx ( 0xff );   					//confirm not in busy state

	sdsc -> sdsc_assert ();
	while ( ! sdsc -> spi_rx () );
	sdsc -> sdsc_deassert ();;

	return 0;
}
