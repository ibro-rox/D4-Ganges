#include "integer.h"
#include "ffconf.h"
#include "ff.h"
#include "diskio.h"





int main(void)
{
	FRESULT f_err_code;
	static FATFS FATFS_Obj;
 
	disk_initialize(0);
	f_err_code = f_mount(0, &FATFS_Obj, 1);
	
	if(f_err_code == 0 || f_err_code == FR_OK)
	{	
		FIL fil_obj;
 
		int variableName = 5;
 
		f_open(&fil_obj, "/moo/foo.txt", FA_WRITE);
 
		f_printf(&fil_obj, "bar %d", variableName);
 
		f_close(&fil_obj);
	}
	
	return 0;

}