/*
 * gpio.c
 *
 *  Created on: Apr 4, 2024
 *      Author: protuberanec
 */

#include "gpio.h"

void gpioC_init() {
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
}
void gpioC_init_for_pwm() {
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1;
}

void gpioC_setBit(int num_bit) {
	GPIOC->BSRR = 1 << num_bit;	//GPIO_BSRR_BS_8;
}

void gpioC_resetBit(int num_bit) {
	GPIOC->BSRR = 1 << (num_bit + 16); //GPIO_BSRR_BR_8;
}

void gpioC_toggleBit(int num_bit) {
	GPIOC->ODR ^= (1 << num_bit);
}


void gpioA_init() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;	//1 << 17
	GPIOA->MODER &= ~GPIO_MODER_MODER3;	//(3 << 0); 0b11 0x03
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_1;	//pull down
	EXTI->IMR |= EXTI_IMR_IM0;
	EXTI->RTSR |= EXTI_RTSR_RT0;

	GPIOA->MODER |= GPIO_MODER_MODER8_1;
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFRH0;

	NVIC_SetPriority(EXTI0_1_IRQn, 8);
	NVIC_EnableIRQ(EXTI0_1_IRQn);
}

int gpioA_getIDR() {
	return GPIOA->IDR;
}

int getIDR(GPIO_TypeDef* gpio) {
	return gpio->IDR;
}

void gpioB_init() {
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER |= GPIO_MODER_MODER12_0;
}


