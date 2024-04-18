/*
 * adc.c
 *
 *  Created on: Apr 18, 2024
 *      Author: protuberanec
 */

#include "adc.h"

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

