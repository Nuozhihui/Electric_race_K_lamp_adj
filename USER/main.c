//引用头文件: 延时、按键、位、串口、屏幕、PWM、ADC
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
*		模块名称	:	主程序模块
*		文件名称	:	main.c
*		版		本	:	V0.1
*		说		明	：控制LED灯,PWM调光
*		修改记录:
*				版本号		日期				作者				说明
*				V0.1			2021-11-26	JINCHANG		初版
*				v0.2			2021-11-28	JINCHANG		新增PWM ADC
*				v0.3			2021-11-28	JINCHANG		对PWM AADC优化 编写README文件(接线引脚通道)
*			Copyright	(C) , 2021-2021 , JINCHANG	blog.hinuohui.com
*******************************************************************************
*/



/********************************************************************************

*		函数名: main()

*		功能：调用函数
·		
*		输入:无

*		输出:无

*		说	明:主程序

*******************************************************************************/

	
 int main(void)
 {	 
	//初始化


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);	 	//串口初始化为115200

	 LED_Init();			      //LED端口初始化
	 LCD_Init();	   				//液晶屏初始化
	 Adc_Init();		  			//ADC初始化

	//初始化PWM
	TIM1_PWM_Init();//不分频。PWM频率=72000/(899+1)=80Khz 

	APP_inint();			//初始化显示界面
	
		 
	//循环
	 while(1)
	 {
		 
		 
		/************************************双路PWM测试********************************/
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
	 

