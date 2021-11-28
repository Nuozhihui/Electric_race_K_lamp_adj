 #include "adc.h"
 #include "delay.h"
 #include "lcd.h"
 #include "gui.h"
	   
 u16 pwmtim1,pwmhl1,pwmtim2,pwmhl2;	
 int Bias[15],Target_value_inint;			//����ƫ��ֵ
 int key_nzw;
	 int i=0;
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PA1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	


	ADC_DeInit(ADC1);  //��λADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת��������ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת������������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 10;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}				  
//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
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

 
 void Bias_ININT(void)		//����ƫ��ֵ����
 {


	 pwmhl1=0;
	 pwmhl2=0;		
	 Bias[0]=Target_value_inint - Get_Adc(0);
	 Bias[1]=Target_value_inint - Get_Adc(1);
	 Bias[2]=Target_value_inint - Get_Adc(2);
	 Bias[3]=Target_value_inint - Get_Adc(3);
	 Bias[4]=Target_value_inint - Get_Adc(4);
	 Bias[5]=Target_value_inint - Get_Adc(5);
	 Bias[6]=Target_value_inint - Get_Adc(6);
	 Bias[7]=Target_value_inint - Get_Adc(7);
	 Bias[8]=Target_value_inint - Get_Adc(10);
	 Bias[9]=Target_value_inint - Get_Adc(11);

	 delay_ms(1000);
	 pwmhl1=0;
	 pwmhl2=0;
 }
 



/********************************************************************************

*		������: APP_show_ADC()

*		��	�ܣ�ҳ����ʾADCֵ����Ļ
��		
*		��	��:	��

*		��	��:	��

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

		
		//��
		 LCD_ShowNum(120,80,date[0],5,18);			//�Ϸ�������		1
		 LCD_ShowNum(220,80,date[1],5,18);			//�Ϸ�������		2
		 LCD_ShowNum(320,80,date[2],5,18);			//�Ϸ�������		3
		 
		 //��
		 LCD_ShowNum(120,270,date[7],5,18);		//�·�������		8
		 LCD_ShowNum(220,270,date[6],5,18);		//�·�������		7
		 LCD_ShowNum(320,270,date[5],5,18);		//�·�������		6
		 
		 //��
		 LCD_ShowNum(25,140,date[9],5,18);			//���Ϸ�����		10
		 LCD_ShowNum(25,210,date[8],5,18);			//���·�����		9
		 
		 
		 //��
		 LCD_ShowNum(405,140,date[3],5,18);		//���Ϸ�����		4
		 LCD_ShowNum(405,210,date[4],5,18);		//���·�����		5
		 
	value1=( date[9]+ date[0]+ date[1]+date[2]+ date[3])/5;
	value2=(date[4]+date[5]+date[6]+date[7]+date[8])/5;
	
		 //ƽ��ֵ
		 LCD_ShowNum(220,145,value1,5,18);		//���Ϸ�����
		 LCD_ShowNum(220,205,value2,5,18);		//���·�����
	 }
 
//	 /*
//		ADCͨ�� 
//		ȡ���ٴ�
//	 */
float smoothing_pj(int aisle , int uum)	//ȥ���ȥ��С��ȡƽ��ֵ
{
	 int a[100];
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


























