/*
 * usart.h
 *
 *  Created on: May 2, 2024
 *      Author: protuberanec
 */

#ifndef USART_H_
#define USART_H_

#include <stm32f0xx.h>

void usart1_init();
void usart1_dma_init();
void usart1_PutChar(uint8_t);
uint8_t usart1_PutChar_1(uint8_t);
uint8_t usart1_GetChar();


#endif /* USART_H_ */
