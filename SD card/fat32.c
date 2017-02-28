
#include "fat32.h"
#include <stdlib.h>

void * read_sector ( unsigned long sector_address, void *buf )
{
	fat -> read_single_block ( sector_address, ( unsigned char * ) buf );
	return buf;
}

unsigned char fat32_init ( void )
{
	boot_sector *bpb;
	partition_info *mbr;
	first_partition_info *partition;

	bpb = ( boot_sector * ) read_sector ( 0, malloc ( 512 ) );

	if ( ( bpb -> jumpboot [ 0 ] != 0xE9 ) && ( bpb -> jumpboot [ 0 ] != 0xEB ) )   //check if it is boot sector
	{																			    //otherwise it must be mbr
		mbr = ( partition_info * ) bpb;
  		
		if ( mbr -> signature != 0xaa55 ) 
			return 1;       													     //not FAT32
		else;

		partition = ( first_partition_info * )( mbr -> partitiondata );			     //first partition
  		fat -> first_used_sector = partition -> firstsector; 								     //the unused sectors, hidden to the FAT	
	
		read_sector ( fat -> first_used_sector, bpb );
		
		if ( ( bpb -> jumpboot [ 0 ] != 0xE9 ) && ( bpb -> jumpboot [ 0 ] != 0xEB ) ) 
			return 1;
		else;
	}else;

	fat -> bytes_per_sector = bpb -> bytespersector;
	fat -> sector_per_cluster = bpb -> sectorpercluster;
	fat -> reserved_sector_count = bpb -> reservedsectorcount;
	fat -> root_cluster = bpb -> rootcluster;
	fat -> first_data_sector = bpb -> hiddensectors + fat -> reserved_sector_count + ( bpb -> numberoffats * bpb -> fatsize_f32 );

	free ( bpb );

	return 0;

}

unsigned long first_sector_of_cluster ( unsigned long cluster_number )
{
  return ( ( ( cluster_number - 2 ) * fat -> sector_per_cluster ) + fat -> first_data_sector );
}

unsigned long first_cluster_of_file ( char *f_name )
{
	unsigned long cluster_number, sector_number;
	unsigned long first_cluster;
	int i, j;
	fs_dir *dir;

	cluster_number = fat -> root_cluster;

	while ( cluster_number < 0x0ffffff6 )
	{	
		for ( sector_number = 0; sector_number < fat -> sector_per_cluster; sector_number ++ )
   		{	
			dir = ( fs_dir * ) read_sector ( 
												(
													first_sector_of_cluster ( cluster_number )
													+ sector_number
												),
												malloc ( fat -> bytes_per_sector )
							    		   );

			for ( i = 0; i < 16; i ++ )
		    {
				for ( j = 0; f_name [ j ] == dir [ i ] . name [ j ]; j ++ );

				if ( ! f_name [ j ] )
				{
					first_cluster = ( ( ( ( unsigned long ) dir [ i ] . firstclusterhi ) << 16 ) | dir [ i ] . firstclusterlo );
					free ( dir );
					return first_cluster;
				}
				else;
			}
			free ( dir );
		}
		cluster_number = next_cluster_number ( cluster_number );
	}
	return 0;
}

unsigned long next_cluster_number ( unsigned long current_cluster_number )
{
	unsigned long fat_sector_for_current_cluster;
	unsigned int offset_of_current_cluster;
	unsigned char *sector_data;
	unsigned long *fat_sector_data;
	unsigned long next_cluster;

	fat_sector_for_current_cluster = fat -> first_used_sector + fat -> reserved_sector_count 
									 + ( ( current_cluster_number * 4 ) / fat -> bytes_per_sector );
	offset_of_current_cluster = ( unsigned int ) ( ( current_cluster_number * 4 ) % fat -> bytes_per_sector );

	sector_data = ( unsigned char * ) read_sector ( 
														    fat_sector_for_current_cluster,
															malloc ( fat -> bytes_per_sector )
							    		   		   );	
	fat_sector_data = ( unsigned long * ) &sector_data [ offset_of_current_cluster ];

	next_cluster = ( ( *fat_sector_data ) & 0x0fffffff );

	free ( sector_data );

	return next_cluster;

}

unsigned char read_file ( char *f_name )
{
	unsigned long cluster, first_sector;
	int i, j;
	char *data;

	cluster = first_cluster_of_file ( f_name );

	while ( 1 )
	{
		first_sector = first_sector_of_cluster ( cluster );
	
		for ( i = 0; i < fat -> sector_per_cluster; i ++ )
	  	{
	    	data = ( char * ) read_sector ( first_sector + i, malloc ( fat -> bytes_per_sector ) );

			for ( j = 0; j < fat -> bytes_per_sector; j ++ )
			{
				if ( 0 == data [ j ] )
				{
					free ( data );
					return 0;
				}else;
				fat -> send_data_byte ( data [ j ] );
			}
			free ( data );
		}
		cluster = next_cluster_number ( cluster );

    }
	return 0;
}

unsigned char list_files ( void )
{
	unsigned long cluster_number, sector_number;
	int i, j;
	fs_dir *dir;

	cluster_number = fat -> root_cluster;

	while ( cluster_number < 0x0ffffff6 )
	{	
		for ( sector_number = 0; sector_number < fat -> sector_per_cluster; sector_number ++ )
   		{	
			dir = ( fs_dir * ) read_sector ( 
												(
													first_sector_of_cluster ( cluster_number )
													+ sector_number
												),
												malloc ( fat -> bytes_per_sector )
							    		   );

			for ( i = 0; i < 16; i ++ )
		    {
				if ( 0x20 == dir [ i ] . attrib )
				{
					for ( j = 0; j < 11; j ++ )
						fat -> send_data_byte ( dir [ i ] . name [ j ] );
					fat -> send_data_byte ( '\n' );
					fat -> send_data_byte ( '\r' );
				}
				else;
			}
			free ( dir );
		}
		cluster_number = next_cluster_number ( cluster_number );
	}
	return 0;
}
