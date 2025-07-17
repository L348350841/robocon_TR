/*    
      ____                      _____                  +---+
     / ___\                     / __ \                 | R |
    / /                        / /_/ /                 +---+
   / /   ________  ____  ___  / ____/___  ____  __   __
  / /  / ___/ __ `/_  / / _ \/ /   / __ \/ _  \/ /  / /
 / /__/ /  / /_/ / / /_/  __/ /   / /_/ / / / / /__/ /
 \___/_/   \__,_/ /___/\___/_/    \___ /_/ /_/____  /
                                                 / /
                                            ____/ /
                                           /_____/
ReceiveData.c file
编写者：小马  (Camel)
作者E-mail：375836945@qq.com
编译环境：MDK-Lite  Version: 4.23
初版时间: 2014-01-28
功能：
1.接收函数文件，包括接收2.4G数据，UART1的数据流
2.解析数据包，分配给对应的控制量
------------------------------------
*/ 
#include "ReceiveData.h"
#include "led.h"
#include "usart.h"
#include "24l01.h"
#include "delay.h"
#include "AllPara.h"

uint16_t rcData[14]={1500,1500,1500,1500};
//RC_GETDATA  RC_DATA;//={0,0,0,0},RC_DATA_RAW={0,0,0,0};	// RC_DATA是处理后的期望四通		question：四通是什么？
extern u8 tmp_buf[33];	

#ifdef ReceiveDataFromNRF

//函数名：ReceiveDataFormNRF()
//输入：无
//输出: 无
//描述：将收到的2.4G遥控数据赋值给对应的变量
//作者：马骏
void ReceiveDataFormNRF(void)
{
		if((tmp_buf[0] == '$')&&(tmp_buf[1] == 'M')&&(tmp_buf[2] == '<'))
		{
				rcData[THROTTLE] = tmp_buf[5] + (tmp_buf[6]<<8);						//将tmp_buf[6]的数据左移8的字节是什么意思？
				rcData[YAW]		   = tmp_buf[7] + (tmp_buf[8]<<8);  
				rcData[PITCH]	   = tmp_buf[9] + (tmp_buf[10]<<8); 
				rcData[ROLL]		 = tmp_buf[11]+ (tmp_buf[12]<<8); 

				rcData[YAW_control]=tmp_buf[13] + (tmp_buf[14]<<8);//角度闭环控制信号
				rcData[aa]=tmp_buf[15] + (tmp_buf[16]<<8);
				rcData[6]=tmp_buf[17] + (tmp_buf[18]<<8); 
				rcData[7]=tmp_buf[19] + (tmp_buf[20]<<8); 
				rcData[8]=tmp_buf[21] + (tmp_buf[22]<<8); 
				rcData[9]=tmp_buf[23] + (tmp_buf[24]<<8); 
				rcData[10]=tmp_buf[25] + (tmp_buf[26]<<8); //转盘转动控制信号

				rcData[11]=tmp_buf[27] ; //拨码开关
				rcData[KEY]=tmp_buf[28] ; //键值
				rcData[13]=tmp_buf[29] ; //mode
				 
		}	
}
/*******************************************************************
* @ref 解码接受数据
* @author 刘帅
* @参数buf：接受的数据
* 使用模拟串口，停用
*******************************************************************/

//void Sbus_Data_Count(uint8_t *buf)
//{
//	
//	sbus_ch[ 0] = ((int16_t)buf[ 2] >> 0 | ((int16_t)buf[ 3] << 8 )) & 0x07FF;
//	sbus_ch[ 1] = ((int16_t)buf[ 3] >> 3 | ((int16_t)buf[ 4] << 5 )) & 0x07FF;
//	sbus_ch[ 2] = ((int16_t)buf[ 4] >> 6 | ((int16_t)buf[ 5] << 2 )  | (int16_t)buf[ 6] << 10 ) & 0x07FF;
//	sbus_ch[ 3] = ((int16_t)buf[ 6] >> 1 | ((int16_t)buf[ 7] << 7 )) & 0x07FF;
//	sbus_ch[ 4] = ((int16_t)buf[ 7] >> 4 | ((int16_t)buf[ 8] << 4 )) & 0x07FF;
//	sbus_ch[ 5] = ((int16_t)buf[ 8] >> 7 | ((int16_t)buf[ 9] << 1 )  | (int16_t)buf[10] <<  9 ) & 0x07FF;
//	sbus_ch[ 6] = ((int16_t)buf[10] >> 2 | ((int16_t)buf[11] << 6 )) & 0x07FF;
//	sbus_ch[ 7] = ((int16_t)buf[11] >> 5 | ((int16_t)buf[12] << 3 )) & 0x07FF;
//	
//	sbus_ch[ 8] = ((int16_t)buf[13] << 0 | ((int16_t)buf[14] << 8 )) & 0x07FF;
//	sbus_ch[ 9] = ((int16_t)buf[14] >> 3 | ((int16_t)buf[15] << 5 )) & 0x07FF;
//	sbus_ch[10] = ((int16_t)buf[15] >> 6 | ((int16_t)buf[16] << 2 )  | (int16_t)buf[17] << 10 ) & 0x07FF;
//	sbus_ch[11] = ((int16_t)buf[17] >> 1 | ((int16_t)buf[18] << 7 )) & 0x07FF;
//	sbus_ch[12] = ((int16_t)buf[18] >> 4 | ((int16_t)buf[19] << 4 )) & 0x07FF;
//	sbus_ch[13] = ((int16_t)buf[19] >> 7 | ((int16_t)buf[20] << 1 )  | (int16_t)buf[21] <<  9 ) & 0x07FF;
//	sbus_ch[14] = ((int16_t)buf[21] >> 2 | ((int16_t)buf[22] << 6 )) & 0x07FF;
//	sbus_ch[15] = ((int16_t)buf[22] >> 5 | ((int16_t)buf[23] << 3 )) & 0x07FF;
//	
//}

/* 该函数用在#define里面 - 停用*/
//u32 change(u16 num)
//{
//   if(num==172)
//		 num = 1;
//	 else if(num==992)
//		 num = 2;
//	 else if(num==1810)
//		 num = 3;
//   else
//		 num = num;

//   return num;
//}

#endif
