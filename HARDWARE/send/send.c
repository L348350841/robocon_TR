#include "send.h"

/*************************************************************************************************
* @brief   	ͨ�����ڷ����ַ�������
 * @version 1.0
 * @date    2020.9.11
 * @param		USARTx����������USART1������λ��ͨ�ź��Ӿ�����
						str		�������������׵�ַ
 * @retval  Usart_SendString(USART1,txt);
 * @author  xb
 *************************************************************************************************/
void Usart_SendString(USART_TypeDef* USARTx,char *str){

		while(*str){
			while(!USART_GetFlagStatus(USARTx,USART_FLAG_TXE)); //�ж��Ƿ���Է���
				USART_SendData(USARTx,*str);
			while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)); //�ж��Ƿ�����ɣ��˾�����У���������ֻ�������һ���ַ������ǣ�
				str++;
		}
		
		
		
		
		
}
//ɽ����λ�����ͺ���
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
 *  @brief      ɽ��๦�ܵ���������λ��������ʾ������ʾ����
 *  @param      wareaddr    ����������ʼ��ַ
 *  @param      waresize    ��������ռ�ÿռ�Ĵ�С
 *  @since      v5.0

		���磺vcan_sendware(USART2,(uint8_t *)var, sizeof(var));
 */
void vcan_sendware(USART_TypeDef* USARTx,void *wareaddr, uint32_t waresize)
{
		
   #define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����

		uart_putbuff(USARTx, cmdf, sizeof(cmdf));    //�ȷ���ǰ����
    uart_putbuff(USARTx, (uint8_t *)wareaddr, waresize);    //��������
    uart_putbuff(USARTx, cmdr, sizeof(cmdr));    //���ͺ�����


}
/*!
 *  @brief      ɽ��๦�ܵ���������λ��������ͷ��ʾ����
 *  @param      imgaddr    ͼ����ʼ��ַ
 *  @param      imgsize    ͼ��ռ�ÿռ�Ĵ�С
 *  @since      v5.0
*  Sample usage:
             �����÷��ο�������:
            ��ɽ������ͷ��ӥ����λ�����̺�΢��Ч�� - ���ܳ�������
             http://vcan123.com/forum.php?mod=viewthread&tid=6242&ctid=27
 */
void vcan_sendimg(USART_TypeDef* USARTx,void *imgaddr, uint32_t imgsize)
{
		#define CMD_IMG     1
    uint8_t cmdf[2] = {CMD_IMG, ~CMD_IMG};    //ɽ����λ�� ʹ�õ�����
    uint8_t cmdr[2] = {~CMD_IMG, CMD_IMG};    //ɽ����λ�� ʹ�õ�����

    uart_putbuff(USARTx, cmdf, sizeof(cmdf));    //�ȷ�������

    uart_putbuff(USARTx, (uint8_t *)imgaddr, imgsize); //�ٷ���ͼ��

    uart_putbuff(USARTx, cmdr, sizeof(cmdr));    //�ȷ�������
}


/*!
 *  @brief      ɽ��๦�ܵ���������λ��������CCD��ʾ����
 *  @param      ccdaddr    CCDͼ����ʼ��ַ
 *  @param      ccdsize    CCDͼ��ռ�ÿռ�Ĵ�С
 *  @since      v5.0
*  Sample usage:
             �����÷��ο�������:
            ��ɽ������CCD����λ����ʾ���̺ͳ���ǲɼ�Ч�� - ���ܳ�������
             http://vcan123.com/forum.php?mod=viewthread&tid=6340&ctid=27
 */
void vcan_sendccd(USART_TypeDef* USARTx,void *ccdaddr, uint32_t ccdsize)
{
		#define CMD_CCD     2
    uint8_t cmdf[2] = {CMD_CCD, ~CMD_CCD};    //��ͷ����
    uint8_t cmdr[2] = {~CMD_CCD, CMD_CCD};    //��β����

    uart_putbuff(USARTx, cmdf, sizeof(cmdf));    //�ȷ�������

    uart_putbuff(USARTx, (uint8_t *)ccdaddr, ccdsize); //�ٷ���ͼ��

    uart_putbuff(USARTx, cmdr, sizeof(cmdr));    //�ٷ�������
}



