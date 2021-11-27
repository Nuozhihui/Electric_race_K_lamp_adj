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
*
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
		 
		 
		/************************************˫·PWM����********************************/
		delay_ms(10);	 
		if(dir)led0pwmval++;
		else led0pwmval--;	 
 		if(led0pwmval>300)dir=0;
		if(led0pwmval==0)dir=1;	   					 
		TIM_SetCompare1(TIM1,led0pwmval);	   
		TIM_SetCompare4(TIM1,led0pwmval);	   
		 

		 DATE();
	}
		 
	 }
	 

