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
*��������:  ADS1115Config
*��	   ���� ����ADS1115����ͨ�����ã�����ʱ��ȵ�
*��	   ���� HCMD �������ָ�8λ(ͨ�������̣�ת��ģʽ)
			LCMD : �����ֵ�8λ(���������� �Ƚ�ģʽ ��Ч��ƽ �ź��������)
*��	   �أ� ��
********************************************************************/
void ADS1115Config_A(u8 HCMD,u8 LCMD)
{
    u8 i=0;
    u8 Initdata[4];

    Initdata[0] = WR_REG;  // ��ַ0x90  ����ADR�ӵ� д�Ĵ���
    Initdata[1] = CONF_REG;// ���üĴ���
    Initdata[2] = HCMD;    // �����ָ��ֽ�
    Initdata[3] = LCMD;    // �����ֵ��ֽ�
    IIC_SCL;
    IIC_Start();        //����
    for(i=0;i<4;i++)
    {
    	IIC_Send_Byte(Initdata[i]);
			IIC_Wait_Ack();
    	//ADS1115_delay(10);
    }
    IIC_Stop();         //�ر�
}

/*******************************************************************
*��������:  SetThresHold_A
*��	   ���� �ߵͷ�������
*��	   ���� H_TH����ֵ��L_TH����ֵ
*��	   �أ� ��
********************************************************************/
/*
void SetThresHold_A(u16 L_TH,u16 H_TH)        //
{
   IIC_SCL;
   I2CStart_A();      // ����
   I2CWriteByte_A(WR_REG);
   I2CWriteByte_A(LOTH_REG);//��ͷ�ֵ�Ĵ���
   I2CWriteByte_A((L_TH>>8));
   I2CWriteByte_A(L_TH);
   I2CStop_A();       //�ر�

   I2CStart_A();     //����
   I2CWriteByte_A(WR_REG);
   I2CWriteByte_A(HITH_REG);//��߷�ֵ�Ĵ���
   I2CWriteByte_A((H_TH>>8));
   I2CWriteByte_A(H_TH);
   I2CStop_A();      //�ر�
}
*/

/*******************************************************************
*��������:  ReadAD_A
*��	   ���� ��ȡADת���Ĵ�����ֵ
*��	   ���� ��ȡ��ֵΪ��ǰ�����õ��Ǹ�ͨ��
*��	   �أ� ��
********************************************************************/
u16 ReadAD_A(void)
{
   u16 Data[2]={0,0};
   IIC_SCL;
   IIC_Start();
   IIC_Send_Byte(WR_REG);//д�Ĵ���
	 IIC_Wait_Ack();
   IIC_Send_Byte(DATA_REG);//ת�����ݼĴ���
	 IIC_Wait_Ack();
   IIC_Stop();
	 
   IIC_Start();
   IIC_Send_Byte(RE_REG);//RE_REG���Ĵ���
	 IIC_Wait_Ack();
   Data[0] = IIC_Read_Byte(1);
   Data[1] = IIC_Read_Byte(1);
   IIC_Stop();
   
   Data[0] = Data[0]<<8 | Data[1];
   return  (Data[0]);//&0x7fff
}
/*******************************************************************
*��������:  getadc
*��	   ���� ���ò���ȡADת���Ĵ�����ֵ
*��	   ���� ch0,ch1,ch2,ch3
*��		 �̣� FSR = +-6.144 V   full scale range
*��	   �أ� ����adcֵ
*�����㹫ʽ�� 6.114*2*value/65535;     //ת���ɵ�ѹ
********************************************************************/
u16 getadc(u16 channel)
{
		float value=0;
    ADS1115Config_A(channel,LCMD1);//����ADS1115����ͨ�����ã�����ʱ��ȵ�
    //delay_nms(100); 
    value=ReadAD_A();//��ȡADת���Ĵ�����ֵ
    return value;
}

