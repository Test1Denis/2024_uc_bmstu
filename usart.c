/*
 * usart.c
 *
 *  Created on: May 2, 2024
 *      Author: protuberanec
 */
#include "usart.h"

static struct RingBuffer rx_buffer;
static char rx_data[RX_DATA_SIZE];

static void rx_buffer_init() {
	ring_init(&rx_buffer, RX_DATA_SIZE, &rx_data[0]);
}

uint8_t usart1_GetDataFromRing(uint8_t *el) {
	return ring_GetFromFront(&rx_buffer, el);
}

void USART1_IRQHandler() {
	if ((USART1->ISR & USART_ISR_TXE) == USART_ISR_TXE) {

	}
	if ((USART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE) {
		char data = USART1->RDR;
		if (ring_PutToEnd(&rx_buffer, data) == -1) {
			//I can't accept your data .... fu... you....
		}
	}
}

static gpio_for_usart_init() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER9_1;	//TX
	GPIOA->MODER |= GPIO_MODER_MODER10_1;	//RX

	GPIOA->AFR[1] |= (1 << 4) | (1 << 8);
}

void usart1_init() {
	rx_buffer_init();
	gpio_for_usart_init();

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
//	USART1->CR1 |= USART_CR1_TXEIE;
	USART1->CR1 |= USART_CR1_RXNEIE;
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;	//
	USART1->BRR = SystemCoreClock / 115200;

	USART1->CR1 |= USART_CR1_UE;

	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART1_IRQn);
}

uint8_t data_tx[256];

void usart1_dma_init() {
	for (int i = 0; i < 256; i++) {
		data_tx[i] = i;
	}
	gpio_for_usart_init();

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
//	USART1->CR1 |= USART_CR1_TXEIE;
//	USART1->CR1 |= USART_CR1_RXNEIE;
	USART1->CR1 |= USART_CR1_TE/* | USART_CR1_RE*/;	//
	USART1->BRR = SystemCoreClock / 115200;

	USART1->CR3 |= USART_CR3_DMAT;

	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel4->CMAR = (uint32_t)&data_tx[0];
	DMA1_Channel4->CPAR = (uint32_t)(&USART1->TDR);
	DMA1_Channel4->CCR |= /*DMA_CCR_CIRC |*/ DMA_CCR_DIR | DMA_CCR_MINC;
	DMA1_Channel4->CNDTR = 256;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_USART1TX_DMA_RMP;

	USART1->CR1 |= USART_CR1_UE;
	DMA1_Channel4->CCR |= DMA_CCR_EN;
}

void usart1_PutChar(uint8_t data) {
	while(!(USART1->ISR & USART_ISR_TXE));
	USART1->TDR = data;
}

uint8_t usart1_PutChar_1(uint8_t data) {
	if(!(USART1->ISR & USART_ISR_TXE)) {
		return 1;
	}
	USART1->TDR = data;
	return 0;
}

uint8_t usart1_GetChar() {
	uint32_t temp_count = 0;
	while(!(USART1->ISR & USART_ISR_RXNE)) {	}

	uint8_t data = USART1->RDR;
	return data;
}






