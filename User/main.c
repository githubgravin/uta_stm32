#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_sdfs_app.h"
#include "bsp_touch.h"
#include "bsp_usart2_485.h"
#include "stdlib.h"
#include "stdio.h"
uint8_t temp[2];
uint16_t temperature;
extern volatile unsigned char touch_flag;
uint16_t Volnum,Vgnnum;

const uint16_t AD605_num[9] = {

	2048,807,869,931,993,1055,1117,1179,1241
};
/*PA4*/
const uint16_t Vol_num[9] = {

	2048,2304,2560,2816,3072,3328,3584,3840,4095
};
void DelayUS(vu32 cnt);
void Screen_Init(void);
void Get_TEMP(void);
void Screen_Touch(uint16_t x, uint16_t y);
void Vgn_Handler(int flag);
void Vol_Handler(int flag);
void Show_Wave(void);
/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{  

		USART1_Config();
	  USART2_Config(); 
		LCD_Init();	
		Touch_Init();
		Sd_fs_init();
		Screen_Init();
		GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
		DelayUS(300000);
		USART_SendData(USART2,0x0a);

	while(1){

		if(0==touch_flag){ 
					Get_TEMP();	
					Show_Wave();
		}else if(1==touch_flag){
					if(1==touch_flag){
							if(Get_touch_point(&display, Read_2046_2(), &touch_para ) !=DISABLE)      
							{				
									Screen_Touch(display.x,display.y);
							}					
					}
		}	
		
	}
	
}
/**
  * @brief  ��ʱ����
  * @param  ��  
  * @retval ��
  */
void DelayUS(vu32 cnt)
{
    uint32_t i;
    for(i = 0;i<cnt;i++)
    {
        uint8_t us = 12; /* ����ֵΪ12����Լ��1΢�� */    
        while (us--)     /* ��1΢��	*/
        {
            ;   
        }
    }
}
/**
  * @brief  ��Ļ��ʼ��
  * @retval ��
  */
void Screen_Init(void){
		Lcd_GramScan(2);	
		LCD_Clear(0, 0, 320,240, BLUE);	
		DelayUS(50000);
		LCD_Clear(0, 0, 320,240, RED);
		DelayUS(50000);
		LCD_Clear(0, 0, 320,240, GREEN);
		DelayUS(50000);
   	LCD_Clear(0, 0, 320,240, YELLOW);
		DelayUS(50000);
		LCD_Clear(0, 0, 320,240, CYAN);
		DelayUS(50000);
		LCD_Clear(0, 0, 320,240, BLACK);
		DelayUS(50000);	
		LCD_Clear(0, 0, 320,240, MAGENTA);
		DelayUS(50000);
		LCD_Clear(0, 0, 320,240, BACKGROUND);
		DelayUS(50000);
   // printf("1");
		while(Touch_Calibrate() !=0);		
		LCD_Clear(0, 0, 320,240, BACKGROUND);
		LCD_DispEnCh(40, 10, "�� �� �� �� �� �� �� �� �� �� ��", RED);
		LCD_Clear(0, 35, 320,3, YELLOW);
		LCD_Clear(260,35, 3,225,YELLOW);	
		LCD_DispEnCh(280, 52, "Vol+", RED);
		LCD_Clear(260,76, 60,3,YELLOW);
		LCD_DispEnCh(280, 93, "Vol-", RED);
		LCD_Clear(260,117, 60,3,YELLOW);
		LCD_DispEnCh(280, 134, "Amp+", RED);
		LCD_Clear(260,158, 60,3,YELLOW);
		LCD_DispEnCh(280, 175, "Amp-", RED);
		LCD_Clear(260,199, 60,3,YELLOW);
		LCD_DispEnCh(275, 210,"¼��", RED);
		LCD_DispEnCh(10, 50,"Temp:      C", RED); 	
		LCD_DispEnCh(64, 50,".", RED); 
		LCD_DispEnCh(10, 90,"����:   %", RED); 
		LCD_DispEnCh(10, 130,"����:    90% ", RED); 		
		LCD_DispEnCh(10, 170,"�Ŵ�ϵ��:     DB", RED); 	
		LCD_DispEnCh(10, 210,"�����ź�ǿ��:", RED); 
}

/**
  * @brief  ��ȡ�¶�
  * @param  ��  
  * @retval ��
  */
void Get_TEMP(void){
	  int i;
		GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
	//	DelayUS(300000);
		USART_SendData(USART2,0x0A);
		while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);	//�������ģʽ	
    for(i=0;i<2;i++){
			while(	USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!= SET); //��ѯֱ��485���յ�����
			temp[i] = USART_ReceiveData(USART2);			
		}
		temperature=temp[0];
		temperature<<=8;
		temperature+=temp[1];
	//	printf("%d",temperature);
			LCD_Clear(50, 50, 16,8,WHITE);
			LCD_DispEnCh(64, 50,".", RED); 
			//LCD_DispStr(50, 50," ", RED);				  
			LCD_DisNum(50, 50,temperature/10, RED);
			LCD_DisNum(68, 50,temperature%10, RED);
		


}
/**
  * @brief 	����VGN��VOL
  * @param  ��  
  * @retval ��
  */

void Screen_Touch(uint16_t x, uint16_t y){
				if(x>=260){
						if((y>=35)&&(y<76)){
									LCD_Clear(263, 38, 77,38,BLACK);
									DelayUS(200000);							
									LCD_Clear(263, 38, 77,38,WHITE);
									LCD_DispEnCh(280, 52, "Vol+", RED);
									Vol_Handler(1);
						}
						else if((y>=76)&&(y<117)){
									LCD_Clear(263,79, 77,38,BLACK);
									DelayUS(200000);								
									LCD_Clear(263,79, 77,38,WHITE);
								  LCD_DispEnCh(280, 93, "Vol-", RED);
									Vol_Handler(0);
						}
						else if((y>=117)&&(y<158)){
									LCD_Clear(263,120, 77,38,BLACK);
									DelayUS(200000);								
									LCD_Clear(263,120, 77,38,WHITE);
									LCD_DispEnCh(280, 134, "Amp+", RED);
									Vgn_Handler(1);
						}
						else if((y>=158)&&(y<199)){
									LCD_Clear(263,161, 77,38,BLACK);
									DelayUS(200000);							
									LCD_Clear(263,161, 77,38,WHITE);
		              LCD_DispEnCh(280, 175, "Amp-", RED);
									Vgn_Handler(0);
						}
						else if((y>=199)&&(y<=240)){
									LCD_Clear(263,202, 77,38,BLACK);
									DelayUS(200000);							
									LCD_Clear(263,202, 77,38,WHITE);
	              	LCD_DispEnCh(275, 210,"¼��", RED);
                //	Record();
									LCD_Clear(160,120,100,16, BACKGROUND);	 
									LCD_DispEnCh(180, 120,"RECORD OVER!", RED);
									DelayUS(1000000);
									LCD_Clear(160,120,100,16, BACKGROUND);
						}
				}





}
/**
  * @brief 	����VOL
  * @param  ��  
  * @retval ��
  */
void Vol_Handler(int flag){
			int m;	
			GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
			USART_SendData(USART2,0x0b);
			while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);	//�������ģʽ	
			//DelayUS(200000);	
			while(	USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!= SET); //��ѯֱ��485���յ�����
			m= USART_ReceiveData(USART2);	
//			GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
//			USART_SendData(USART2,Volnum);


				printf("%x\r\n",m);
			if(1==flag){
						if(m<8){
						GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
						//USART_SendData(USART2,0x0b);
						//while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);
						++m;
						Volnum=m;
						USART_SendData(USART2,m);
						while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);	
						
						}else if(m==8){
								GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
								USART_SendData(USART2,m);
								while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);
						    Volnum=m;							
								LCD_DispEnCh(180, 120,"��������", RED);
								DelayUS(1000000);
								LCD_Clear(160,120,100,16, BACKGROUND);
						}
				}else if(0==flag){
						if(m==0){
								GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
								USART_SendData(USART2,m);
								while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);	
						    Volnum=m;
								LCD_DispEnCh(180, 120,"��������", RED);	
								DelayUS(1000000);
								LCD_Clear(160,120,100,16, BACKGROUND);
						}else if(m>0){
								GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
								--m;
								Volnum=m;
								USART_SendData(USART2,m);
								while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);

						
						}
				}
}
/**
  * @brief 	����VGN
  * @param  ��  
  * @retval ��
  */
void Vgn_Handler(int flag){

			int m;
	
			GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
			USART_SendData(USART2,0x0C);
			while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);	//�������ģʽ	
		//	DelayUS(200000);	
			while(	USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!= SET); //��ѯֱ��485���յ�����
			m= USART_ReceiveData(USART2);	
//			GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
//			USART_SendData(USART2,Vgnnum);

			if(1==flag){
						if(m<8){
							  GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
							  ++m;
								Vgnnum=m;
								USART_SendData(USART2,m);
						while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);	
						}else if(m==8){
								GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
								USART_SendData(USART2,m);
								while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);	
								Vgnnum=m;
								LCD_DispEnCh(180, 120,"�Ŵ�����", RED);
								DelayUS(2000000);
								LCD_Clear(160,120,100,16, BACKGROUND);
						}
				}else if(0==flag){
						if(m==0){
								GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
								USART_SendData(USART2,m);
								while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);
			          Vgnnum=m;							
								LCD_DispEnCh(180, 120,"�Ŵ�����", RED);	
								DelayUS(2000000);
								LCD_Clear(160,120,100,16, BACKGROUND);
						}else if(m>0){
								GPIO_SetBits(GPIOB,GPIO_Pin_5);	//����ģʽ
								--m;	
								Vgnnum=m;
								USART_SendData(USART2,m);
								while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);	
						}
				}
}


void Show_Wave(void){
			
			float Vol,Vgn;
			Vgn=40*((float)AD605_num[Vgnnum]/4096*3.3)-24;
			LCD_Clear(90,170,16,16, BACKGROUND);
			LCD_DisNum(90, 170,Vgn, RED);
			Vol=(float)Vol_num[Volnum]/4096*100;
			LCD_DisNum(50, 90,Vol, RED);
}
/* ----------------------------------------end of file------------------------------------------------ */
