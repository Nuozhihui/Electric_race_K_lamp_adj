//����ͷ�ļ�: ��ʱ��������λ�����ڡ���Ļ��PWM��ADC
#include "delay.h"		
//#include "key.h"
#include "led.h"
#include "sys.h"
#include "usart.h"	 
#include "adc.h"
#include "lcd.h"
#include "touch.h"
#include "gui.h"
#include "test.h"
#include "PWM.h"
#include "APP1.h"

 	u16 led0pwmval=0;
	u8 dir=1;	
	

/*
*******************************************************************************
*		ģ������	:	������ģ��
*		�ļ�����	:	main.c
*		��		��	:	V0.1
*		˵		��	������LED��,PWM����
*		�޸ļ�¼:
*				�汾��		����				����				˵��
*				V0.1			2021-11-26	JINCHANG		����
*				v0.2			2021-11-28	JINCHANG		����PWM ADC
*				v0.3			2021-11-28	JINCHANG		��PWM AADC�Ż� ��дREADME�ļ�(��������ͨ��)
				v0.31			2021-11-29	JINCHANG		��ADC��PWM�����Ż� ---(�˰汾Ϊ���԰�)
*			Copyright	(C) , 2021-2021 , JINCHANG	blog.hinuohui.com
*******************************************************************************
*/



/********************************************************************************

*		������: main()

*		���ܣ����ú���
��		
*		����:��

*		���:��

*		˵	��:������

*******************************************************************************/

	
 int main(void)
 {	 
	//��ʼ��


		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		delay_init();	    	 //��ʱ������ʼ��	  
		uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200

	 LED_Init();			      //LED�˿ڳ�ʼ��
	 LCD_Init();	   				//Һ������ʼ��
	 Adc_Init();		  			//ADC��ʼ��

	//��ʼ��PWM
	TIM1_PWM_Init();//����Ƶ��PWMƵ��=72000/(899+1)=80Khz 

	APP_inint();			//��ʼ����ʾ����
	
		 
	//ѭ��
	 while(1)
	 {
		 
//		 
//		/************************************˫·PWM����********************************/
//		delay_ms(10);	 
//		if(dir)led0pwmval++;
//		else led0pwmval--;	 
// 		if(led0pwmval>1000)dir=0;
//		if(led0pwmval==0)dir=1;	   					 
//		TIM_SetCompare1(TIM1,led0pwmval);	   
//		TIM_SetCompare4(TIM1,led0pwmval);	   
//		 
//		//���ݴ���
//		DATE();
		//		/************************************ADC�㷨���Բ���********************************/ 
		 
		  led0pwmval=(int)smoothing_pj(0 ,3)*0.244;
		 LCD_ShowNum(220,145, led0pwmval,5,18);		//���Ϸ�����
		 	TIM_SetCompare1(TIM1,led0pwmval);	   
		  TIM_SetCompare4(TIM1,led0pwmval);	 
		 LCD_ShowNum(220,205,0,5,18);		//���·�����
	}
		 
	 }
	 

