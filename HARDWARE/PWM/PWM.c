#include "stm32f10x.h"
#include "PWM.h"

u16 ARR = 899;//自动重装值
u16 PSC = 0;//时钟预分频系数
u16 CCR = 4;//待装入捕获比较寄存器的脉冲值
void TIM1_GPIO_Config (void)
{
GPIO_InitTypeDef GPIO_InitStructure; 	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 使能TIM1的时钟，即内部时钟FCK_INT=72M
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA外设时钟

//设置该引脚为复用输出功能,输出TIM1_CH1（OC1）的PWM脉冲波形
//输出比较通道GPIO初始化,PA8	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11; //TIM1_CH1对应GOIO为PA8
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出，因为使用了PWM，PWM是为端口复用，所以要设置成AF
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化以上配置
}


/*------配置高级定时器的工作模式------*/
void TIM1_Mode_Config (void)
{	
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定义一个TimeBaseInitTypeDef类型的变量
TIM_OCInitTypeDef  TIM_OCInitStructure;
	
//1*--------------------时基结构体初始化--------------------*/
/*时基初始化,配置TIM1的时钟输出频率，以及其它相关参数初始化*/
TIM_TimeBaseStructure.TIM_Period = ARR;//自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或中断 	
TIM_TimeBaseStructure.TIM_Prescaler = PSC; //驱动计数器CNT的时钟，它的时钟等于FCK_INT/(PSC+1)
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//设置时钟分频系数。0：不分频；TIM_CKD_DIV1:设置时钟分频系数，1分频	
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器计数模式：向上计数模式
TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //初始化定时器
//---------------------------------------------------------------------------------------------//


	
//2*--------------------输出比较结构体初始化--------------------*/
/*设置PWM的输出方式,PWM模式配置，通道1：OC1*/
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //输出模式配置，主要有PWM1，PWM2。这里：选择定时器模式:TIM脉冲宽度调制模式2
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //配置输出模式状态：使能或关闭。这里：比较输出使能
TIM_OCInitStructure.TIM_Pulse = CCR; //设置待装入捕获比较寄存器的脉冲值 
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //设置有效电平极性，把PWM模式的有效电平设置为高电平或低电平
	
TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx	
//设置使能寄存器TIM_CCMR1的OC1PE位，这一位是开启TIM1_CCR1寄存器的预装载功能
//TIMx_CCRx寄存器能够在任何时候通过软件进行更新以控制波形，这个通过软件写入控制波形的值是立即生效呢或在定时器发生下一次更新事件时被更新的
//Enable就是下一次更新事件时被更新；Disable是立即生效
TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);			
TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);			
//---------------------------------------------------------------------------------------------//	



//3*--------------------使能--------------------*/ 			
//允许或禁止在定时器工作时向ARR的缓冲器中写入新值，以便在更新事件发生时载入覆盖以前的值。
//在开始初始化的时候你已经把" TIM_TimeBaseStructure.TIM_Period=PSC;"，后来也一直是这个值。
//由于没有编写中断服务函数或者你在中断服务函数中根本就没有给ARR缓冲器重新写入新值,
//所以TIM_ARRPreloadConfig()设置为DISABLE 和ENABLE都没有影响。
TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIM1在ARR上的预装载寄存器
		
TIM_Cmd(TIM1, ENABLE);  //使能TIM1
TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	

}

 
void TIM1_PWM_Init(void)
{  
TIM1_GPIO_Config();//对作为TIM外设通道复用的GPIO引脚初始化
TIM1_Mode_Config ();//对TIM外设初始化
}	
