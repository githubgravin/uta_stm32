#ifndef __BSP_TOUCH_H
#define	__BSP_TOUCH_H

#include "stm32f10x.h"

/* Һ������ṹ�� */
typedef	struct POINT 
{
   uint16_t x;		
   uint16_t y;
}Coordinate;

/*  У��ϵ���ṹ�� */
typedef struct Parameter 
{						
long double An,  			 //ע:sizeof(long double) = 8
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
}Parameter ;

extern volatile unsigned char touch_flag;
extern uint8_t cal_flag;
extern long double cal_p[6];

extern Coordinate ScreenSample[4];
extern Coordinate DisplaySample[4];
extern Parameter touch_para ;
extern Coordinate  display ;

#define	CHX 	0x90 	//ͨ��Y+��ѡ�������	
#define	CHY 	0xd0	//ͨ��X+��ѡ�������


void Touch_Init(void);
int Touch_Calibrate(void);
void Palette_Init(void);
FunctionalState Get_touch_point(Coordinate * displayPtr,
                                Coordinate * screenPtr,
                                Parameter * para );
Coordinate *Read_2046_2(void);
void Palette_draw_point(uint16_t x, uint16_t y);

void Touch_Test(void);

#endif /* __BSP_TOUCH_H */

