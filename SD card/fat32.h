#ifndef _FAT32_H_
#define _FAT32_H_

#define READ_ONLY     		0x01
#define HIDDEN        		0x02
#define SYSTEM        		0x04
#define VOLUME_ID     		0x08
#define DIRECTORY     		0x10
#define ARCHIVE       		0x20
#define LONG_NAME     		0x0f
#define DIR_ENTRY_SIZE     	0x32
#define EMPTY              	0x00
#define DELETED            	0xe5
#define GET     		   	0
#define SET                	1
#define READ	           	0
#define VERIFY             	1
#define ADD		           	0
#define REMOVE	           	1
#define LOW		           	0
#define HIGH	           	1	
#define TOTAL_FREE         	1
#define NEXT_FREE          	2
#define GET_LIST           	0
#define GET_FILE           	1
#define DELETE		       	2
#define FAT_EOF	           	0x0fffffff

typedef struct 
		{
			unsigned char	ignore [ 446 ];							//ignore, placed here to fill the gap in the structure
			unsigned char	partitiondata [ 64 ];					//partition records (16x4)
			unsigned int	signature;								//0xaa55
		}									partition_info;

typedef struct
		{  				
			unsigned char	status;									//0x80 - active partition
			unsigned char 	headstart;								//starting head
			unsigned int	cylsectstart;							//starting cylinder and sector
			unsigned char	type;									//partition type 
			unsigned char	headend;								//ending head of the partition
			unsigned int	cylsectend;								//ending cylinder and sector
			unsigned long	firstsector;							//total sectors between MBR & the first sector of the partition
			unsigned long	sectorstotal;							//size of this partition in sectors
		}									first_partition_info;

typedef struct
		{
			unsigned char jumpboot [ 3 ]; 							//default: 0x009000EB
			unsigned char oemname [ 8 ];
			unsigned int bytespersector; 							//deafault: 512
			unsigned char sectorpercluster;
			unsigned int reservedsectorcount;
			unsigned char numberoffats;
			unsigned int rootentrycount;
			unsigned int totalsectors_f16; 							//must be 0 for FAT32
			unsigned char mediatype;
			unsigned int fatsize_f16; 								//must be 0 for FAT32
			unsigned int sectorspertrack;
			unsigned int numberofheads;
			unsigned long hiddensectors;
			unsigned long totalsectors_F32;
			unsigned long fatsize_f32; 								//count of sectors occupied by one FAT
			unsigned int extflags;
			unsigned int fsversion; 								//0x0000 (defines version 0.0)
			unsigned long rootcluster; 								//first cluster of root directory (=2)
			unsigned int fsinfo; 									//sector number of FSinfo structure (=1)
			unsigned int backupbootsector;
			unsigned char reserved [ 12 ];
			unsigned char drivenumber;
			unsigned char reserved1;
			unsigned char bootsignature;
			unsigned long volumeid;
			unsigned char volumelabel [ 11 ]; 						//"NO NAME "
			unsigned char filesystemtype [ 8 ];						//"FAT32"
			unsigned char bootdata [ 420 ];
			unsigned int bootendsignature; 							//0xaa55
		}										boot_sector;

typedef struct
		{
			unsigned long leadsignature;   							//0x41615252
			unsigned char reserved1 [ 480 ];
			unsigned long structuresignature;						//0x61417272
			unsigned long freeclustercount; 						//initial: 0xffffffff
			unsigned long nextfreecluster; 							//initial: 0xffffffff
			unsigned char reserved2 [ 12 ];
			unsigned long trailsignature; 							//0xaa550000
		}										fs_info;

typedef struct
		{
			unsigned char name [ 11 ];
			unsigned char attrib; 									//file attributes
			unsigned char ntreserved; 								//always 0
			unsigned char timetenth; 								//tenths of seconds, set to 0 here
			unsigned int createtime; 								//time file was created
			unsigned int createdate; 								//date file was created
			unsigned int lastaccessdate;
			unsigned int firstclusterhi; 							//higher word of the first cluster number
			unsigned int writetime; 								//time of last write
			unsigned int writedate; 								//date of last write
			unsigned int firstclusterlo; 							//lower word of the first cluster number
			unsigned long filesize; 								//size of file in bytes
		}										fs_dir;

typedef struct
		{
			unsigned long first_used_sector;
			volatile unsigned long first_data_sector, root_cluster;
			volatile unsigned int  bytes_per_sector, sector_per_cluster, reserved_sector_count;
			void ( *read_single_block ) ( unsigned long block_address, unsigned char *buf );
			void ( *send_data_byte ) ( unsigned char send );
		}										fat32_interface;

fat32_interface *fat;

void * read_sector ( unsigned long sector_address, void *buf );
unsigned char fat32_init ( void );
unsigned long first_sector_of_cluster ( unsigned long cluster_number );
unsigned long first_cluster_of_file ( char *f_name );
unsigned long next_cluster_number ( unsigned long current_cluster_number );
unsigned char read_file ( char *f_name );
unsigned char list_files ( void );

#endif
