#ifndef __SEND_H_
#define __SEND_H_

#include "sys.h"
#include "usart.h"
void Usart_SendString(USART_TypeDef* USARTx,char *str);
void vcan_sendimg(USART_TypeDef* USARTx,void *imgaddr, uint32_t imgsize);
void vcan_sendccd(USART_TypeDef* USARTx,void *ccdaddr, uint32_t ccdsize);
void vcan_sendware(USART_TypeDef* USARTx,void *wareaddr, uint32_t waresize);
void uart_putbuff (USART_TypeDef* USARTx, uint8_t *buff, uint32_t len);

#endif 
