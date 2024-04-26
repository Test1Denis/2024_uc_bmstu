/*
 * adc.c
 *
 *  Created on: Apr 18, 2024
 *      Author: protuberanec
 */

#include "adc.h"

int adc_data_for_dma[1024];

static void init_gpio_as_adc() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER4 | GPIO_MODER_MODER5;
}

void init_ADC() {
	init_gpio_as_adc();
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->CR2 |= RCC_CR2_HSI14ON;

	ADC1->CR &= ~ADC_CR_ADDIS;

//	ADC1->IER |= ADC_IER_EOCIE /* | ADC_IER_OVRIE*/;
	ADC1->CFGR1 |= /*ADC_CFGR1_CONT | */ADC_CFGR1_SCANDIR;
	ADC1->CHSELR |= ADC_CHSELR_CHSEL5 | ADC_CHSELR_CHSEL4;

	ADC1->CFGR1 |= ADC_CFGR1_OVRMOD;

	ADC1->CFGR1 |= ADC_CFGR1_RES_1;

//	NVIC_SetPriority(ADC1_IRQn, 0x02);
//	NVIC_EnableIRQ(ADC1_IRQn);

	ADC1->CR |= ADC_CR_ADEN;

//	ADC1->CR |= ADC_CR_ADSTART;	//turn of need set adc_cr_adstop
}

void init_ADC_with_dma() {
	init_gpio_as_adc();
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->CR2 |= RCC_CR2_HSI14ON;

	ADC1->CR &= ~ADC_CR_ADDIS;

//	ADC1->IER |= ADC_IER_EOCIE /* | ADC_IER_OVRIE*/;
	ADC1->CFGR1 |= ADC_CFGR1_CONT | ADC_CFGR1_SCANDIR;
	ADC1->CHSELR |= ADC_CHSELR_CHSEL5 | ADC_CHSELR_CHSEL4;
	ADC1->CFGR1 |= ADC_CFGR1_DMACFG | ADC_CFGR1_DMAEN;

	ADC1->CFGR1 |= ADC_CFGR1_OVRMOD;

	ADC1->CFGR1 |= ADC_CFGR1_RES_1;

	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel1->CNDTR = 1024;

	DMA1_Channel1->CMAR = &adc_data_for_dma[0];
	DMA1_Channel1->CPAR = (uint32_t*)&ADC1->DR;
	DMA1_Channel1->CCR |= DMA_CCR_MINC | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_CIRC | DMA_CCR_TCIE;

	NVIC_SetPriority(DMA1_Ch1_IRQn, 0x01);
	NVIC_EnableIRQ(DMA1_Ch1_IRQn);


	ADC1->CR |= ADC_CR_ADEN;
	DMA1_Channel1->CCR |= DMA_CCR_EN;

	ADC1->CR |= ADC_CR_ADSTART;	//turn of need set adc_cr_adstop
}

