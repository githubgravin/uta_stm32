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
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{  

		USART1_Config();
	  USART2_Config(); 
		LCD_Init();	
		Touch_Init();
		Sd_fs_init();
		Screen_Init();
		GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
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
  * @brief  延时函数
  * @param  无  
  * @retval 无
  */
void DelayUS(vu32 cnt)
{
    uint32_t i;
    for(i = 0;i<cnt;i++)
    {
        uint8_t us = 12; /* 设置值为12，大约延1微秒 */    
        while (us--)     /* 延1微秒	*/
        {
            ;   
        }
    }
}
/**
  * @brief  屏幕初始化
  * @retval 无
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
		LCD_DispEnCh(40, 10, "基 于 超 声 波 局 部 放 电 检 测", RED);
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
		LCD_DispEnCh(275, 210,"录音", RED);
		LCD_DispEnCh(10, 50,"Temp:      C", RED); 	
		LCD_DispEnCh(64, 50,".", RED); 
		LCD_DispEnCh(10, 90,"音量:   %", RED); 
		LCD_DispEnCh(10, 130,"电量:    90% ", RED); 		
		LCD_DispEnCh(10, 170,"放大系数:     DB", RED); 	
		LCD_DispEnCh(10, 210,"接收信号强度:", RED); 
}

/**
  * @brief  获取温度
  * @param  无  
  * @retval 无
  */
void Get_TEMP(void){
	  int i;
		GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
	//	DelayUS(300000);
		USART_SendData(USART2,0x0A);
		while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);	//进入接收模式	
    for(i=0;i<2;i++){
			while(	USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!= SET); //轮询直到485接收到数据
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
  * @brief 	设置VGN和VOL
  * @param  无  
  * @retval 无
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
	              	LCD_DispEnCh(275, 210,"录音", RED);
                //	Record();
									LCD_Clear(160,120,100,16, BACKGROUND);	 
									LCD_DispEnCh(180, 120,"RECORD OVER!", RED);
									DelayUS(1000000);
									LCD_Clear(160,120,100,16, BACKGROUND);
						}
				}





}
/**
  * @brief 	设置VOL
  * @param  无  
  * @retval 无
  */
void Vol_Handler(int flag){
			int m;	
			GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
			USART_SendData(USART2,0x0b);
			while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);	//进入接收模式	
			//DelayUS(200000);	
			while(	USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!= SET); //轮询直到485接收到数据
			m= USART_ReceiveData(USART2);	
//			GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
//			USART_SendData(USART2,Volnum);


				printf("%x\r\n",m);
			if(1==flag){
						if(m<8){
						GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
						//USART_SendData(USART2,0x0b);
						//while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);
						++m;
						Volnum=m;
						USART_SendData(USART2,m);
						while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);	
						
						}else if(m==8){
								GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
								USART_SendData(USART2,m);
								while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);
						    Volnum=m;							
								LCD_DispEnCh(180, 120,"音量上限", RED);
								DelayUS(1000000);
								LCD_Clear(160,120,100,16, BACKGROUND);
						}
				}else if(0==flag){
						if(m==0){
								GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
								USART_SendData(USART2,m);
								while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);	
						    Volnum=m;
								LCD_DispEnCh(180, 120,"音量下限", RED);	
								DelayUS(1000000);
								LCD_Clear(160,120,100,16, BACKGROUND);
						}else if(m>0){
								GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
								--m;
								Volnum=m;
								USART_SendData(USART2,m);
								while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);

						
						}
				}
}
/**
  * @brief 	设置VGN
  * @param  无  
  * @retval 无
  */
void Vgn_Handler(int flag){

			int m;
	
			GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
			USART_SendData(USART2,0x0C);
			while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);	//进入接收模式	
		//	DelayUS(200000);	
			while(	USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!= SET); //轮询直到485接收到数据
			m= USART_ReceiveData(USART2);	
//			GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
//			USART_SendData(USART2,Vgnnum);

			if(1==flag){
						if(m<8){
							  GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
							  ++m;
								Vgnnum=m;
								USART_SendData(USART2,m);
						while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);	
						}else if(m==8){
								GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
								USART_SendData(USART2,m);
								while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);	
								Vgnnum=m;
								LCD_DispEnCh(180, 120,"放大上限", RED);
								DelayUS(2000000);
								LCD_Clear(160,120,100,16, BACKGROUND);
						}
				}else if(0==flag){
						if(m==0){
								GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
								USART_SendData(USART2,m);
								while (USART_GetFlagStatus(USART2,USART_FLAG_TC) !=SET);
			          Vgnnum=m;							
								LCD_DispEnCh(180, 120,"放大下限", RED);	
								DelayUS(2000000);
								LCD_Clear(160,120,100,16, BACKGROUND);
						}else if(m>0){
								GPIO_SetBits(GPIOB,GPIO_Pin_5);	//发送模式
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
