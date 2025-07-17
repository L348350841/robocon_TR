#include "ADS1115.h" 
#include "delay.h"
#include "usart.h"	
/************************************************************************************************************/

void delay_nms(u16 ms)
{
	u16 i;
	u32 M = 0;//720W
	for(i = 0;i < ms; i++)
	for(M=2333;M > 0;M--);
}

/*********************************************************************
*函数名称:  ADS1115Config
*描	   述： 设置ADS1115包括通道配置，采样时间等等
*参	   数： HCMD ：命令字高8位(通道，量程，转换模式)
			LCMD : 命令字低8位(采样率设置 比较模式 有效电平 信号输出锁存)
*返	   回； 无
********************************************************************/
void ADS1115Config_A(u8 HCMD,u8 LCMD)
{
    u8 i=0;
    u8 Initdata[4];

    Initdata[0] = WR_REG;  // 地址0x90  器件ADR接地 写寄存器
    Initdata[1] = CONF_REG;// 配置寄存器
    Initdata[2] = HCMD;    // 配置字高字节
    Initdata[3] = LCMD;    // 配置字低字节
    IIC_SCL;
    IIC_Start();        //开启
    for(i=0;i<4;i++)
    {
    	IIC_Send_Byte(Initdata[i]);
			IIC_Wait_Ack();
    	//ADS1115_delay(10);
    }
    IIC_Stop();         //关闭
}

/*******************************************************************
*函数名称:  SetThresHold_A
*描	   述： 高低阀门设置
*参	   数： H_TH高阈值，L_TH低阈值
*返	   回； 无
********************************************************************/
/*
void SetThresHold_A(u16 L_TH,u16 H_TH)        //
{
   IIC_SCL;
   I2CStart_A();      // 开启
   I2CWriteByte_A(WR_REG);
   I2CWriteByte_A(LOTH_REG);//最低阀值寄存器
   I2CWriteByte_A((L_TH>>8));
   I2CWriteByte_A(L_TH);
   I2CStop_A();       //关闭

   I2CStart_A();     //开启
   I2CWriteByte_A(WR_REG);
   I2CWriteByte_A(HITH_REG);//最高阀值寄存器
   I2CWriteByte_A((H_TH>>8));
   I2CWriteByte_A(H_TH);
   I2CStop_A();      //关闭
}
*/

/*******************************************************************
*函数名称:  ReadAD_A
*描	   述： 获取AD转换寄存器的值
*参	   数： 获取的值为在前面设置的那个通道
*返	   回； 无
********************************************************************/
u16 ReadAD_A(void)
{
   u16 Data[2]={0,0};
   IIC_SCL;
   IIC_Start();
   IIC_Send_Byte(WR_REG);//写寄存器
	 IIC_Wait_Ack();
   IIC_Send_Byte(DATA_REG);//转换数据寄存器
	 IIC_Wait_Ack();
   IIC_Stop();
	 
   IIC_Start();
   IIC_Send_Byte(RE_REG);//RE_REG读寄存器
	 IIC_Wait_Ack();
   Data[0] = IIC_Read_Byte(1);
   Data[1] = IIC_Read_Byte(1);
   IIC_Stop();
   
   Data[0] = Data[0]<<8 | Data[1];
   return  (Data[0]);//&0x7fff
}
/*******************************************************************
*函数名称:  getadc
*描	   述： 配置并获取AD转换寄存器的值
*参	   数： ch0,ch1,ch2,ch3
*量		 程： FSR = +-6.144 V   full scale range
*返	   回； 返回adc值
*最后计算公式： 6.114*2*value/65535;     //转换成电压
********************************************************************/
u16 getadc(u16 channel)
{
		float value=0;
    ADS1115Config_A(channel,LCMD1);//设置ADS1115包括通道配置，采样时间等等
    //delay_nms(100); 
    value=ReadAD_A();//获取AD转换寄存器的值
    return value;
}

