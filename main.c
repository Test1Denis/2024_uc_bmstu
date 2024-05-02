/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 9.3.0   2024-04-04

The MIT License (MIT)
Copyright (c) 2019 STMicroelectronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************
*/

/* Includes */
#include "stm32f0xx.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "usart.h"
#include "ds18b20.h"
/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

extern int adc_data_for_dma[1024];

void init_blink() {
	gpioC_init();
}

void blinkLed() {
	gpioC_toggleBit(8);
}

volatile int count_press_button = 0;

void EXTI0_1_IRQHandler() {
	int count = TIM2->CNT;
	if ((EXTI->PR & EXTI_PR_PIF0) == EXTI_PR_PIF0) {
		EXTI->PR |= EXTI_PR_PIF0;
		count_press_button++;
	}

	TIM2->CNT = 0;
}

void TIM6_DAC_IRQHandler() {
	TIM6->SR &= ~TIM_SR_UIF;
	blinkLed();
	GPIOB->ODR ^= GPIO_ODR_12;
}

uint16_t res_adc;

void ADC1_COMP_IRQHandler() {
	if ((ADC1->ISR & ADC_ISR_EOC) == ADC_ISR_EOC) {
		res_adc = ADC1->DR;
	}
	else if ((ADC1->ISR & ADC_ISR_OVR) == ADC_ISR_OVR) {
//		ADC1->ISR |= ADC_ISR_OVR;
	}
	else if ((ADC1->ISR & ADC_ISR_EOSEQ) == ADC_ISR_EOS) {
		ADC1->ISR |= ADC_ISR_EOS;
	}
}

void DMA1_Channel1_IRQHandler(void) {
	if ((DMA1->ISR & DMA_ISR_TCIF1) == DMA_ISR_HTIF1) {
		DMA1->IFCR |= DMA_IFCR_CHTIF1;
	}

	if ((DMA1->ISR & DMA_ISR_TCIF1) == DMA_ISR_TCIF1) {
		DMA1->IFCR |= DMA_IFCR_CTCIF1;
		GPIOC->ODR ^= GPIO_ODR_8;
	}

}
uint8_t data = 0;
void USART1_IRQHandler() {
	if ((USART1->ISR & USART_ISR_TXE) == USART_ISR_TXE) {
		USART1->TDR = data;
		++data;
	}

}

void CheckDrebezg() {
	if (count_press_button > 0) {
		blinkLed();
		for (int i = 0; i < 50000; i++);
		blinkLed();
		for (int i = 0; i < 50000; i++);
		count_press_button--;
	}
}

void Set48MHz() {
//	SystemCoreClock; <-- здесь хранится текущая тактовая частота, после вызова функции SytemCoreClockUpdate()
	RCC->CR &= ~RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) == RCC_CR_PLLRDY);

	RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2;
	RCC->CFGR |= RCC_CFGR_PLLMUL12;
	RCC->CR |= RCC_CR_PLLON;	//turn on
	while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

	RCC->CFGR |= RCC_CFGR_SW_1;
	while((RCC->CFGR & RCC_CFGR_SWS_1) != RCC_CFGR_SWS_1);

	SystemCoreClockUpdate();
}

uint16_t adc_data1;
uint16_t adc_data2;

void ReadADC_ch1() {
	ADC1->CR |= ADC_CR_ADSTP;
	while((ADC1->CR & ADC_CR_ADSTP) == ADC_CR_ADSTP);
	ADC1->CHSELR = ADC_CHSELR_CHSEL4;
	ADC1->CR |= ADC_CR_ADSTART;
	while((ADC1->ISR & ADC_ISR_EOC) != ADC_ISR_EOC);
	adc_data1 = ADC1->DR;
}

void ReadADC_ch2() {
	ADC1->CR |= ADC_CR_ADSTP;
	while((ADC1->CR & ADC_CR_ADSTP) == ADC_CR_ADSTP);
	ADC1->CHSELR = ADC_CHSELR_CHSEL5;
	ADC1->CR |= ADC_CR_ADSTART;
	while((ADC1->ISR & ADC_ISR_EOC) != ADC_ISR_EOC);
	adc_data2 = ADC1->DR;
}

void SetPwm_1() {
	TIM3->CCR3 = adc_data_for_dma[0];
}
void SetPwm_2() {
	TIM3->CCR4 = adc_data_for_dma[1];
}

int main(void)
{
//	Set48MHz();
//	gpioA_init();
//	gpioB_init();
//	gpioC_init_for_pwm();
//	tim3_init_as_pwm();
//	tim6_init();
//	tim6_start();
//	init_ADC_with_dma();
//	usart1_init();
//	ds18b20_Init(GPIOC, 13, DS18B20_RES_12);

	usart1_dma_init();
	while(1) {

	}

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER13_0;

	volatile float a = 10321421;
//	int a = 0;
	while (1)
	{
		GPIOC->BSRR = GPIO_BSRR_BS_13;
		for (int i = 0; i < 1000; i++) {
			a += 3;
			a += 3;
			a += 3;
			a += 3;
			a += 3;
			a += 3;
			a += 3;
			a += 3;
			a += 3;
			a += 3;
		}
		GPIOC->BSRR = GPIO_BSRR_BR_13;
		for (int i = 0; i < 100; i++);

//		ds18b20_StartMeas();
//		for (int i = 0; i < 1000; i++) {
//			Delay_tick(3400);
//		}
////		while(!ds18b20_isConversion());
//
//		temperature = ds18b20_GetTemperature();
//		real_temp = (temperature & 0xFFF) * 0.0625;
	}
}

/*
 * 	EXTI->IMR |= EXTI_IMR_IM0;
	EXTI->RTSR |= EXTI_RTSR_RT0;
	EXTI->FTSR |= EXTI_FTSR_FT0;
	NVIC_SetPriority(EXTI0_1_IRQn, 0x00);
	NVIC_EnableIRQ(EXTI0_1_IRQn);
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->ARR = 0xFFFFFFFF;
	TIM2->PSC = 0;
	TIM2->CR1 |= TIM_CR1_CEN;

	TIM2->CNT = 0x00;
	EXTI->SWIER |= EXTI_SWIER_SWI0;
//	NVIC_SetPendingIRQ(EXTI0_1_IRQn);
 */

