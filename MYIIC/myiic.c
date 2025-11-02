#include "myiic.h"
#include "delay.h" // 需要一个延时函数，通常是systick或简单循环延时

// 初始化IIC
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置SCL和SDA引脚
    GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN | IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // 开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 初始化后将SCL和SDA都拉高，进入空闲状态
    IIC_SCL_H();
    IIC_SDA_H();
}

// 产生IIC起始信号
void IIC_Start(void)
{
    IIC_SDA_H();
    IIC_SCL_H();
    Delay_us(4); // 延时确保时序稳定
    IIC_SDA_L(); // START: when CLK is high, DATA change form high to low 
    Delay_us(4);
    IIC_SCL_L(); // 钳住I2C总线，准备发送或接收数据
}	

// 产生IIC停止信号
void IIC_Stop(void)
{
    IIC_SCL_L();
    IIC_SDA_L(); // STOP: when CLK is high, DATA change form low to high
    Delay_us(4);
    IIC_SCL_H(); 
    IIC_SDA_H(); 
    Delay_us(4);							   	
}

// 等待应答信号到来
// 返回值：1，接收应答失败
//         0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    IIC_SDA_H(); // 释放SDA，准备接收ACK
    Delay_us(1);	   
    IIC_SCL_H();
    Delay_us(1);	 
    while(READ_SDA())
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_L(); // 时钟输出0 	   
    return 0;  
} 

// 产生ACK应答
void IIC_Ack(void)
{
    IIC_SCL_L();
    IIC_SDA_L();
    Delay_us(2);
    IIC_SCL_H();
    Delay_us(2);
    IIC_SCL_L();
}

// 不产生ACK应答		
void IIC_NAck(void)
{
    IIC_SCL_L();
    IIC_SDA_H();
    Delay_us(2);
    IIC_SCL_H();
    Delay_us(2);
    IIC_SCL_L();
}

// IIC发送一个字节
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
    IIC_SCL_L(); // 拉低时钟开始数据传输
    for(t = 0; t < 8; t++)
    {              
        if((txd & 0x80) >> 7) // 判断最高位是1还是0
            IIC_SDA_H();
        else
            IIC_SDA_L();
        txd <<= 1; 	  
        Delay_us(2);   
        IIC_SCL_H();
        Delay_us(2); 
        IIC_SCL_L();	
        Delay_us(2);
    }	 
} 	    

// 读1个字节，ack=1时，发送ACK，ack=0，发送NACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    for(i = 0; i < 8; i++ )
    {
        IIC_SCL_L(); 
        Delay_us(2);
        IIC_SCL_H();
        receive <<= 1;
        if(READ_SDA()) receive++;   
        Delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck(); // 发送nACK
    else
        IIC_Ack(); // 发送ACK   
    return receive;
}

