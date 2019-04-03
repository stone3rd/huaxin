//#include <stm32f10x_lib.h>
#include "stm32f10x.h"
#include "key.h"
#include "delay.h"
//Mini STM32������
//�������� ��������			 
//����ԭ��@ALIENTEK
//2010/5/27   

//������ʼ������
//�ر�ע�⣺�ڸú���֮��JTAG���޷�ʹ�ã�SWDҲ�޷�ʹ�ã�
//�����JTAG���棬�������θú�����
//PA0.13.15 ���ó�����
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	GPIOA->CRL&=0XFFFFFFF0;//PA0���ó�����	  
	GPIOA->CRL|=0X00000008;   
	GPIOA->CRH&=0X0F0FFFFF;//PA13,15���ó�����	  
	GPIOA->CRH|=0X80800000; 				   
	GPIOA->ODR|=1<<13;	   //PA13����,PA0Ĭ������
	GPIOA->ODR|=1<<15;	   //PA15����
} 
//������������
//���ذ���ֵ
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2!!
u8 KEY_Scan(void)
{	 
	static u8 key_up=1;//�������ɿ���־	
	JTAG_Set(JTAG_SWD_DISABLE);
	if(key_up&&(KEY0==0||KEY1==0||KEY2==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)
		{
			JTAG_Set(SWD_ENABLE);
			return 1;
		}
		else if(KEY1==0)
		{
			JTAG_Set(SWD_ENABLE);
			return 2;
		}
		else if(KEY2==1)
		{
			JTAG_Set(SWD_ENABLE);
			return 3;
		}
	}else if(KEY0==1&&KEY1==1&&KEY2==0)key_up=1; 	    
	JTAG_Set(SWD_ENABLE);
	return 0;// �ް�������
}



















