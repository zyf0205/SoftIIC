#ifndef __MYIIC_H
#define __MYIIC_H

#include "stm32f10x.h"

// 定义SCL和SDA连接的GPIO端口和引脚
#define IIC_SCL_PORT    GPIOB
#define IIC_SCL_PIN     GPIO_Pin_6
#define IIC_SDA_PORT    GPIOB
#define IIC_SDA_PIN     GPIO_Pin_7

// IO操作函数宏定义
#define IIC_SCL_H()     GPIO_SetBits(IIC_SCL_PORT, IIC_SCL_PIN)
#define IIC_SCL_L()     GPIO_ResetBits(IIC_SCL_PORT, IIC_SCL_PIN)
#define IIC_SDA_H()     GPIO_SetBits(IIC_SDA_PORT, IIC_SDA_PIN)
#define IIC_SDA_L()     GPIO_ResetBits(IIC_SDA_PORT, IIC_SDA_PIN)
#define READ_SDA()      GPIO_ReadInputDataBit(IIC_SDA_PORT, IIC_SDA_PIN)

// IIC所有操作函数
void IIC_Init(void);                // 初始化IIC的IO口				 
void IIC_Start(void);				// 发送IIC开始信号
void IIC_Stop(void);	  			// 发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			// IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);	// IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 			// IIC等待ACK信号
void IIC_Ack(void);					// IIC发送ACK信号
void IIC_NAck(void);				// IIC不发送ACK信号

#endif
