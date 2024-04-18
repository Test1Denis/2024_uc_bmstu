/*
 * tim.c
 *
 *  Created on: Apr 11, 2024
 *      Author: protuberanec
 */

#include "tim.h"



void tim6_init() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->ARR = 4800 - 1;
	TIM6->PSC = 100 - 1;

	TIM6->DIER |= TIM_DIER_UIE;
	NVIC_SetPriority(TIM6_DAC_IRQn, 3);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);

}
void tim6_start() {
	TIM6->CR1 |= TIM_CR1_CEN;
}
void tim6_stop() {
	TIM6->CR1 &= ~TIM_CR1_CEN;
}

void tim3_init_as_pwm() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	//SystemCoreClock
	TIM3->ARR = 255;
	TIM3->PSC = 1000;
//channel 3
	TIM3->CCMR2 |= TIM_CCMR2_OC3CE;
	TIM3->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0;	//<-- pwm mode 2
	TIM3->CCR3 = 0;	//<-- here need put pwm value...
	TIM3->CCER |= TIM_CCER_CC3E;

//channel 4
	TIM3->CCMR2 |= TIM_CCMR2_OC4CE;
	TIM3->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;	//<-- pwm mode 1
	TIM3->CCR4 = 0;	//<-- here need put pwm value...

	TIM3->CCER |= TIM_CCER_CC4E;


	TIM3->CR1 |= TIM_CR1_CEN;
}



