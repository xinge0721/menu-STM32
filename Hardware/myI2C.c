#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define MyI2C_W_SCL(x) (GPIO_WriteBit(GPIOA,GPIO_Pin_6,x))
#define MyI2C_W_SDA(x) (GPIO_WriteBit(GPIOA,GPIO_Pin_7,x))
#define MyI2C_R_SDA()  (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11))

void MyI2C_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);	
}

void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{	
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
	for(int i = 0 ; i < 8 ; i++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t data = 0x00;
	MyI2C_W_SDA(1);							
	for(int i = 0 ; i < 8 ; i++)
	{		
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA()) data |= (0x80 >> i);
		MyI2C_W_SCL(0);
	}
	return data;
}

void MyI2C_SendAck(uint8_t AckBit)
{
		MyI2C_W_SCL(AckBit);
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t data = 0;
	MyI2C_W_SDA(1);							
	MyI2C_W_SCL(1);
	if(MyI2C_R_SDA()) data = 1;
	MyI2C_W_SCL(0);
	return data;
}
