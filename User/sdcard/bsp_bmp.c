#include "bsp_bmp.h"
#include "ff.h"
#include "bsp_ili9341_lcd.h"

#define RGB24TORGB16(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5)	| ((B)>>3)))

BYTE pColorData[960];					/* һ�����ɫ���ݻ��� 320 * 3 = 960 */
//tagRGBQUAD dataOfBmp[17*19];
FATFS bmpfs[2]; 
FIL bmpfsrc, bmpfdst; 
FRESULT bmpres;

/* �������Ҫ��ӡbmp��ص���ʾ��Ϣ,��printfע�͵�����
 * ��Ҫ��printf()���轫���������ļ���������
 */
#define BMP_DEBUG_PRINTF(FORMAT,...)  //printf(FORMAT,##__VA_ARGS__)	 

/* ��ӡBMP�ļ���ͷ��Ϣ�����ڵ��� */
static void showBmpHead(BITMAPFILEHEADER* pBmpHead)
{
    BMP_DEBUG_PRINTF("λͼ�ļ�ͷ:\r\n");
    BMP_DEBUG_PRINTF("�ļ���С:%d\r\n",(*pBmpHead).bfSize);
    BMP_DEBUG_PRINTF("������:%d\r\n",(*pBmpHead).bfReserved1);
    BMP_DEBUG_PRINTF("������:%d\r\n",(*pBmpHead).bfReserved2);
    BMP_DEBUG_PRINTF("ʵ��λͼ���ݵ�ƫ���ֽ���:%d\r\n",(*pBmpHead).bfOffBits);
		BMP_DEBUG_PRINTF("\r\n");	
}

/* ��ӡBMP�ļ���ͷ��Ϣ�����ڵ��� */
static void showBmpInforHead(tagBITMAPINFOHEADER* pBmpInforHead)
{
    BMP_DEBUG_PRINTF("λͼ��Ϣͷ:\r\n");
    BMP_DEBUG_PRINTF("�ṹ��ĳ���:%d\r\n",(*pBmpInforHead).biSize);
    BMP_DEBUG_PRINTF("λͼ��:%d\r\n",(*pBmpInforHead).biWidth);
    BMP_DEBUG_PRINTF("λͼ��:%d\r\n",(*pBmpInforHead).biHeight);
    BMP_DEBUG_PRINTF("biPlanesƽ����:%d\r\n",(*pBmpInforHead).biPlanes);
    BMP_DEBUG_PRINTF("biBitCount������ɫλ��:%d\r\n",(*pBmpInforHead).biBitCount);
    BMP_DEBUG_PRINTF("ѹ����ʽ:%d\r\n",(*pBmpInforHead).biCompression);
    BMP_DEBUG_PRINTF("biSizeImageʵ��λͼ����ռ�õ��ֽ���:%d\r\n",(*pBmpInforHead).biSizeImage);
    BMP_DEBUG_PRINTF("X����ֱ���:%d\r\n",(*pBmpInforHead).biXPelsPerMeter);
    BMP_DEBUG_PRINTF("Y����ֱ���:%d\r\n",(*pBmpInforHead).biYPelsPerMeter);
    BMP_DEBUG_PRINTF("ʹ�õ���ɫ��:%d\r\n",(*pBmpInforHead).biClrUsed);
    BMP_DEBUG_PRINTF("��Ҫ��ɫ��:%d\r\n",(*pBmpInforHead).biClrImportant);
		BMP_DEBUG_PRINTF("\r\n");
}

/*
 * ��ʾbmpͼƬ, 24λ���ɫ
 * ͼƬ��Ⱥ͸߶ȸ���ͼƬ��С����
 */
void Lcd_show_bmp(unsigned short int x, unsigned short int y,unsigned char *pic_name)
{
	int i, j, k;
	int width, height, l_width;

	BYTE red,green,blue;
	BITMAPFILEHEADER bitHead;
	BITMAPINFOHEADER bitInfoHead;
	WORD fileType;

	unsigned int read_num;
	unsigned char tmp_name[20];
	sprintf((char*)tmp_name,"0:%s",pic_name);

	f_mount(3, &bmpfs[0]);
	BMP_DEBUG_PRINTF("file mount ok \r\n");    
	bmpres = f_open( &bmpfsrc , (char *)tmp_name, FA_OPEN_EXISTING | FA_READ);	
/*-------------------------------------------------------------------------------------------------------*/
	if(bmpres == FR_OK)
	{
		BMP_DEBUG_PRINTF("Open file success\r\n");

		/* ��ȡ�ļ�ͷ��Ϣ  �����ֽ�*/         
		f_read(&bmpfsrc,&fileType,sizeof(WORD),&read_num);     

		/* �ж��ǲ���bmp�ļ� "BM"*/
		if(fileType != 0x4d42)
		{
			BMP_DEBUG_PRINTF("file is not .bmp file!\r\n");
			return;
		}
		else
		{
			BMP_DEBUG_PRINTF("Ok this is .bmp file\r\n");	
		}        

		/* ��ȡBMP�ļ�ͷ��Ϣ*/
		f_read(&bmpfsrc,&bitHead,sizeof(tagBITMAPFILEHEADER),&read_num);        
		showBmpHead(&bitHead);

		/* ��ȡλͼ��Ϣͷ��Ϣ */
		f_read(&bmpfsrc,&bitInfoHead,sizeof(BITMAPINFOHEADER),&read_num);        
		showBmpInforHead(&bitInfoHead);
	}    
	else
	{
		BMP_DEBUG_PRINTF("file open fail!\r\n");
		return;
	}    
/*-------------------------------------------------------------------------------------------------------*/
	width = bitInfoHead.biWidth;
	height = bitInfoHead.biHeight;

	/* ����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ32�ı���	*/
	l_width = WIDTHBYTES(width* bitInfoHead.biBitCount);	

	if(l_width > 960)
	{
		BMP_DEBUG_PRINTF("\n SORRY, PIC IS TOO BIG (<=320)\n");
		return;
	}
	
	/* ����Lcd Gram ɨ�跽��Ϊ: ���½�->���Ͻ� */
	Lcd_GramScan( 3 );
	
	/* ��һ��ͼƬ��С�Ĵ���*/
	LCD_OpenWindow(x, y, width, height);

	
	/* �ж��Ƿ���24bit���ɫͼ */
	if(bitInfoHead.biBitCount >= 24)
	{
		for(i=0;i< height; i++)
		{	  
			/* ��ȡһ��bmp�����ݵ�����pColorData���� */
			#if 0
				for(j=0; j< l_width; j++)	 													
				{                
					f_read(&bmpfsrc,pColorData+j,1,&read_num);
				}            
			#elif 1				
				f_read(&bmpfsrc,pColorData,l_width/2,&read_num);
				f_read(&bmpfsrc,pColorData+l_width/2,l_width/2,&read_num);
			#else
				f_read(&bmpfsrc,pColorData,l_width,&read_num);
			#endif

			for(j=0; j<width; j++) 											   //һ����Ч��Ϣ
			{
				k = j*3;																	 //һ���е�K�����ص����
				red = pColorData[k+2];
				green = pColorData[k+1];
				blue = 	pColorData[k];
				LCD_WR_Data(RGB24TORGB16(red,green,blue)); //д��LCD-GRAM
			}            
		}        	 																					     
	}    
	else 
	{        
		BMP_DEBUG_PRINTF("SORRY, THIS PIC IS NOT A 24BITS REAL COLOR");
		return ;
	}
	f_close(&bmpfsrc);    
}









