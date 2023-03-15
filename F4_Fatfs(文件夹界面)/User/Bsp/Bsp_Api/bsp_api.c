#include "bsp_api.h"
 
FATFS Fs;
FIL File1;
DIR File_Dir;
FILINFO File_Fno;
TCHAR* File_Name;
uint8_t Dir_Flag;

const uint8_t Wq_Tx_Buff[1024]=" 刚好经过几个";
uint8_t Wq_Rx_Buff[1024]="ssssss";
char Dir_Path[]="0:";

BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */

uint32_t Br=12;
uint32_t Bw=12;

FRESULT scan_files (char* path);
FRESULT TFT_Scan_Files (char* path);

void HAL_MspInit(void)
{

	LED_Pin_Init();
	Key_Init();
	USART1_Init_Config();
  NT35510_Init();
	Fatfs_Init();
}


void Fatfs_Init(void)
{
	uint8_t Res_Flash;	
	
	Res_Flash=f_mount(&Fs,"0:",1);

	if(Res_Flash==FR_NO_FILESYSTEM)
	{
	  printf("》FLASH 还没有文件系统，即将进行格式化...\r\n");
		Res_Flash=f_mkfs("0:",NULL,work,FF_MAX_SS);
		
		if (Res_Flash == FR_OK) 
		{
			printf("》FLASH 已成功格式化文件系统。\r\n");
			/* 格式化后，先取消挂载 */
			Res_Flash = f_mount(NULL,"0:",0);
			/* 重新挂载 */
			Res_Flash = f_mount(&Fs,"0:",1);
		} 
		else 
		{
			printf("《《格式化失败。》》\r\n");
			while (1);
		}
		
	}
	 else if (Res_Flash==FR_INVALID_DRIVE ) 
	 {
			printf("！！外部 Flash 挂载文件系统失败。(%d)\r\n",Res_Flash);
			printf("！！可能原因：SPI Flash 初始化不成功。在路径名中指定了无效的驱动器号，或者将空指针指定为路径名\r\n");
		 
			while (1);
	 }
	 else 
	 {
		  printf("！！Res_Flash(%d)\r\n",Res_Flash);
	    printf("》文件系统挂载成功，可以进行读写测试\r\n");
	 }
	 
	 
/*	 
		Res_Flash = f_opendir(&File_Dir, "0:/");  //创建目录                     
		printf("Res_Flash=%d\n",Res_Flash);
		f_closedir(&File_Dir);
	 
*/
/*	 
		f_mkdir("0:/Sub1");//生成子目录
		Res_Flash=f_open(&File1,"0:/Sub1/F03.TXT", FA_WRITE|FA_OPEN_ALWAYS);//创建子目录文件
		Res_Flash=f_write(&File1,&Wq_Tx_Buff,300,&Bw);
		f_close(&File1);
		printf("f_size=%d\n",f_size(&File1));	
*/	 


	 
/* 创建/写入文件
		Res_Flash=f_open(&File1,"0:F02.TXT", FA_WRITE|FA_OPEN_ALWAYS);
		Res_Flash=f_write(&File1,&Wq_Tx_Buff,1024,&Bw);
		f_close(&File1);
		printf("f_size=%d\n",f_size(&File1));			
		
		Res_Flash=f_open(&File1,"0:F04.TXT", FA_WRITE|FA_OPEN_ALWAYS);
		Res_Flash=f_write(&File1,&Wq_Tx_Buff,1024,&Bw);
		f_close(&File1);
		
		Res_Flash=f_open(&File1,"0:F05.jbg", FA_WRITE|FA_OPEN_ALWAYS);
		Res_Flash=f_write(&File1,&gImage_tt,479048,&Bw);
  	f_close(&File1);		
			
		Res_Flash=f_open(&File1,"0:F01.jbg", FA_READ);
		TFT_Dis_FsImage(300,100,283,100,56608);
		f_close(&File1);
*/		

/* 读取目录成员
		f_opendir(&File_Dir, "0:/");                      
		Res_Flash = f_readdir(&File_Dir, &File_Fno);   
		printf("fname=%s\n",File_Fno.fname);

		Res_Flash = f_readdir(&File_Dir, &File_Fno);   
		printf("fname=%s\n",File_Fno.fname);

		Res_Flash = f_readdir(&File_Dir, &File_Fno);   
		printf("fname=%s\n",File_Fno.fname);
		f_closedir(&File_Dir);
*/
//	  TFT_Scan_Files(Dir_Path);//读取目录成员
}

FRESULT scan_files (char* path)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;

    res = f_opendir(&dir, path);                       /* 打开目录 */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /*读取目录项  子目录/文件 */
            if (res != FR_OK || fno.fname[0] == 0) break;  /*在错误或dir 文件结束*/
					
            if (fno.fattrib & AM_DIR) {                    /* 它是一个目录 */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path);                    /* 进入子目录 */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* 普通文件. */
                printf("%s/%s\n", path, fno.fname);
							  
            }
        }
        f_closedir(&dir);
    }
		else
		{
		 printf("res=%d\n",res);
		}

    return res;
}
//扫描并显示目录
FRESULT TFT_Scan_Files (char* path)
{
    FRESULT res;
    UINT i=0;
	  uint16_t usY=50;
	   

    res = f_opendir(&File_Dir, path);                       /* 打开目录 */
    if (res == FR_OK) {
        for (;;) {
                res = f_readdir(&File_Dir, &File_Fno);                   /*读取目录项  子目录/文件 */
            if (res != FR_OK || File_Fno.fname[0] == 0) break;  /*在错误或dir 文件结束*/  
					
//            if (File_Fno.fattrib & AM_DIR) {                    /* 它是一个目录 */
//                printf("%s/%s\n", path, File_Fno.fname);
//            } else {                                       /* 普通文件. */
//                printf("%s/%s\n", path, File_Fno.fname);
//					      printf("f_size=%d\n",File_Fno.fsize);							  
//            }
					      TFT_Dis_Str(0,usY,(uint8_t*)File_Fno.fname,0xf800,0x2c4a);//显示目录成员
								usY+=40;
            }
				  f_closedir(&File_Dir);
        }
			else
			{
			 printf("res=%d\n",res);
			}

    return res;
}
//在某个目录下选择需要的文件和目录名称
FRESULT TFT_Slate_Files (char* path,uint16_t Num)
{
    FRESULT res;
    UINT i=0;
	   
    res = f_opendir(&File_Dir, path);                       /* 打开目录 */
    if (res == FR_OK) {
        for (;;) {
                res = f_readdir(&File_Dir, &File_Fno);                   /*读取目录项  子目录/文件 */
            if (res != FR_OK || File_Fno.fname[0] == 0) break;  /*在错误或dir 文件结束*/  
					
            if (File_Fno.fattrib & AM_DIR) /* 它是一个目录 */
						{  
              Dir_Flag=1;							
            }
						else   /* 普通文件. */
						{                                     
              Dir_Flag=0;					
            }
 					    if(i==Num)//选择文件序号
							{
							  File_Name=File_Fno.fname;
								f_closedir(&File_Dir);
								break;
							}
							i++;						

            }
						 f_closedir(&File_Dir);
        }
			else
			{
			 printf("res=%d\n",res);
			}

    return res;
}

void Dir_Frist_Gui(void)//根目录界面
{
	uint16_t i,UsY=0;
  NT35510_Draw_Win(0,0,300,480,0x2c4a);
	NT35510_Draw_Win(301,0,500,480,0x5555);
	TFT_Dis_Chinaese(0,5,"驱动器根目录：",0xf800,0x2c4a);
	for(i=0;i<12;i++)
	{
    NT35510_Draw_Win(0,UsY,300,2,0xf800);
		UsY+=40;
	}
	TFT_Scan_Files(Dir_Path);
}

void Dir_Sub1_Gui(void)//某子目录界面 需要动态编写 Sub1,Sub2....
{
	uint16_t i,UsY=0;
  NT35510_Draw_Win(0,0,300,480,0x2c4a);
	NT35510_Draw_Win(301,0,500,480,0x5555);
	TFT_Dis_Str(0,10,"SUB1/:",0xf800,0x2c4a);
	for(i=0;i<12;i++)
	{
    NT35510_Draw_Win(0,UsY,300,2,0xf800);
		UsY+=40;
	}
	TFT_Scan_Files("0:/Sub1");
}







