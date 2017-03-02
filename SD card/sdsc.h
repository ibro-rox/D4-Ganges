
#ifndef _SDSC_H_
#define _SDSC_H_

#define GO_IDLE_STATE            0
#define SEND_OP_COND             1
#define SEND_IF_COND			 8
#define SEND_CSD                 9
#define STOP_TRANSMISSION        12
#define SEND_STATUS              13
#define SET_BLOCK_LEN            16
#define READ_SINGLE_BLOCK        17
#define READ_MULTIPLE_BLOCKS     18
#define WRITE_SINGLE_BLOCK       24
#define WRITE_MULTIPLE_BLOCKS    25
#define ERASE_BLOCK_START_ADDR   32
#define ERASE_BLOCK_END_ADDR     33
#define ERASE_SELECTED_BLOCKS    38
#define SD_SEND_OP_COND			 41   
#define APP_CMD					 55
#define READ_OCR				 58
#define CRC_ON_OFF               59

typedef struct
		{
			void ( *spi_init ) ( void );
			void ( *spi_init_2x ) ( void );
			void ( *spi_tx ) ( unsigned char data );
			unsigned char ( *spi_rx ) ( void );
			void ( *sdsc_assert ) ( void );
			void ( *sdsc_deassert ) ( void );
		}											sdsc_interface;

sdsc_interface *sdsc;

unsigned char sdsc_command ( unsigned char cmd, unsigned long arg );
void sdsc_init ( void );
void read_single_block ( unsigned long block_address, unsigned char *buf );
unsigned char write_single_block ( unsigned long block_address, unsigned char *buf );

#endif
