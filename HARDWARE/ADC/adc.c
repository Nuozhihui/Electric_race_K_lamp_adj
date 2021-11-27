 #include "adc.h"
 #include "delay.h"
 #include "lcd.h"
 #include "gui.h"
	   
 u16 pwmtim1,pwmhl1,pwmtim2,pwmhl2;	
 int Bias[15],Target_value_inint;			//光敏偏差值
 int key_nzw;
	 int i=0;
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	


	ADC_DeInit(ADC1);  //复位ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 10;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}				  
//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 

 
// void Bias_ININT(void)		//光敏偏差值测算
// {


//	 pwmhl1=0;
//	 pwmhl2=0;		
//	 Bias[0]=Target_value_inint - Get_Adc(0);
//	 Bias[1]=Target_value_inint - Get_Adc(1);
//	 Bias[2]=Target_value_inint - Get_Adc(2);
//	 Bias[3]=Target_value_inint - Get_Adc(3);
//	 Bias[4]=Target_value_inint - Get_Adc(4);
//	 Bias[5]=Target_value_inint - Get_Adc(5);
//	 Bias[6]=Target_value_inint - Get_Adc(6);
//	 Bias[7]=Target_value_inint - Get_Adc(7);
//	 Bias[8]=Target_value_inint - Get_Adc(10);
//	 Bias[9]=Target_value_inint - Get_Adc(11);
//	//	 printf("A:%d P:%d\n",Get_Adc(0),Bias[0]);	 
//	 delay_ms(1000);
//	 pwmhl1=0;
//	 pwmhl2=0;
// }
// 



/********************************************************************************

*		函数名: APP_show_ADC()

*		功	能：页面显示ADC值到屏幕
·		
*		输	入:	无

*		输	出:	无

*******************************************************************************/
 void DATE()
 { 
	int value1;
	int value2;
	int date[20];


	 
	 date[0]=smoothing_pj(0 ,10)+ Bias[0];
	 date[1]=smoothing_pj(1 ,10)+ Bias[1];
	 date[2]=smoothing_pj(2 ,10)+ Bias[2];
	 date[3]=smoothing_pj(3 ,10)+ Bias[3];
	 date[4]=smoothing_pj(4 ,10)+ Bias[4];
	 date[5]=smoothing_pj(5 ,10)+ Bias[5];
	 date[6]=smoothing_pj(6 ,10)+ Bias[6];
	 date[7]=smoothing_pj(7 ,10)+ Bias[7];
	 date[8]=smoothing_pj(10 ,10)+Bias[8];
	 date[9]=smoothing_pj(11 ,10)+Bias[9];

		
		//上
		 LCD_ShowNum(120,80,date[0],5,18);			//上方左数字		1
		 LCD_ShowNum(220,80,date[1],5,18);			//上方中数字		2
		 LCD_ShowNum(320,80,date[2],5,18);			//上方右数字		3
		 
		 //下
		 LCD_ShowNum(120,270,date[7],5,18);		//下方左数字		8
		 LCD_ShowNum(220,270,date[6],5,18);		//下方中数字		7
		 LCD_ShowNum(320,270,date[5],5,18);		//下方右数字		6
		 
		 //左
		 LCD_ShowNum(25,140,date[9],5,18);			//左上方数字		10
		 LCD_ShowNum(25,210,date[8],5,18);			//左下方数字		9
		 
		 
		 //右
		 LCD_ShowNum(405,140,date[3],5,18);		//右上方数字		4
		 LCD_ShowNum(405,210,date[4],5,18);		//右下方数字		5
		 
	value1=( date[9]+ date[0]+ date[1]+date[2]+ date[3])/5;
	value2=(date[4]+date[5]+date[6]+date[7]+date[8])/5;
	
		 //平均值
		 LCD_ShowNum(220,145,value1,5,18);		//右上方数字
		 LCD_ShowNum(220,205,value2,5,18);		//右下方数字
	 }
 
//	 /*
//		ADC通道 
//		取多少次
//	 */
float smoothing_pj(int aisle , int uum)	//去最大，去最小，取平均值
{
	 int a[20];
	int i,max,min,sum;
	double average;

	 
	for(i=0;i<uum;i++)
	{
		
	a[i]=Get_Adc(aisle);
		
	}
	
	sum=0;
	max=a[0];
	min=a[0];
	
for(i=0;i<uum;i++)

{
	if(max<a[i]){
		
		max=a[i];
		
	}
	if(min>a[i]){
		min=a[i];
	}
	
	sum=sum+a[i];

}

average=(double)(sum-max-min)/(uum-2);

return average;

 
		 
	 }


























