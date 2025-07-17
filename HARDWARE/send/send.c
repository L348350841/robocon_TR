#include "send.h"

/*************************************************************************************************
* @brief   	通过串口发送字符串函数
 * @version 1.0
 * @date    2020.9.11
 * @param		USARTx：串口数，USART1用于上位机通信和视觉调试
						str		：待发送数组首地址
 * @retval  Usart_SendString(USART1,txt);
 * @author  xb
 *************************************************************************************************/
void Usart_SendString(USART_TypeDef* USARTx,char *str){

		while(*str){
			while(!USART_GetFlagStatus(USARTx,USART_FLAG_TXE)); //判断是否可以发送
				USART_SendData(USARTx,*str);
			while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)); //判断是否发送完成，此句必须有，否则会造成只发送最后一个字符（覆盖）
				str++;
		}
		
		
		
		
		
}
//山外上位机发送函数
void uart_putbuff (USART_TypeDef* USARTx, uint8_t *buff, uint32_t len)
{
    while(len--)
    {
		USART_SendData(USARTx, *buff);
		while( USART_GetFlagStatus(USARTx,USART_FLAG_TC)!= SET);  
		buff++;
    }
}

/*!
 *  @brief      山外多功能调试助手上位机，虚拟示波器显示函数
 *  @param      wareaddr    波形数组起始地址
 *  @param      waresize    波形数组占用空间的大小
 *  @since      v5.0

		例如：vcan_sendware(USART2,(uint8_t *)var, sizeof(var));
 */
void vcan_sendware(USART_TypeDef* USARTx,void *wareaddr, uint32_t waresize)
{
		
   #define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

		uart_putbuff(USARTx, cmdf, sizeof(cmdf));    //先发送前命令
    uart_putbuff(USARTx, (uint8_t *)wareaddr, waresize);    //发送数据
    uart_putbuff(USARTx, cmdr, sizeof(cmdr));    //发送后命令


}
/*!
 *  @brief      山外多功能调试助手上位机，摄像头显示函数
 *  @param      imgaddr    图像起始地址
 *  @param      imgsize    图像占用空间的大小
 *  @since      v5.0
*  Sample usage:
             具体用法参考这帖子:
            【山外摄像头】鹰眼上位机例程和微焦效果 - 智能车资料区
             http://vcan123.com/forum.php?mod=viewthread&tid=6242&ctid=27
 */
void vcan_sendimg(USART_TypeDef* USARTx,void *imgaddr, uint32_t imgsize)
{
		#define CMD_IMG     1
    uint8_t cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    uint8_t cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令

    uart_putbuff(USARTx, cmdf, sizeof(cmdf));    //先发送命令

    uart_putbuff(USARTx, (uint8_t *)imgaddr, imgsize); //再发送图像

    uart_putbuff(USARTx, cmdr, sizeof(cmdr));    //先发送命令
}


/*!
 *  @brief      山外多功能调试助手上位机，线性CCD显示函数
 *  @param      ccdaddr    CCD图像起始地址
 *  @param      ccdsize    CCD图像占用空间的大小
 *  @since      v5.0
*  Sample usage:
             具体用法参考这帖子:
            【山外线性CCD】上位机显示例程和超广角采集效果 - 智能车资料区
             http://vcan123.com/forum.php?mod=viewthread&tid=6340&ctid=27
 */
void vcan_sendccd(USART_TypeDef* USARTx,void *ccdaddr, uint32_t ccdsize)
{
		#define CMD_CCD     2
    uint8_t cmdf[2] = {CMD_CCD, ~CMD_CCD};    //开头命令
    uint8_t cmdr[2] = {~CMD_CCD, CMD_CCD};    //结尾命令

    uart_putbuff(USARTx, cmdf, sizeof(cmdf));    //先发送命令

    uart_putbuff(USARTx, (uint8_t *)ccdaddr, ccdsize); //再发送图像

    uart_putbuff(USARTx, cmdr, sizeof(cmdr));    //再发送命令
}



