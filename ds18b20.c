/*
 * ds18b20.c
 *
 *  Created on: 14 дек. 2020 г.
 *      Author: Tr
 */

#include "ds18b20.h"

GPIO_TypeDef *CurrGPIO;
uint16_t curr_pin;

#define SET_OUT()	(CurrGPIO->MODER |= 1 << curr_pin * 2)
#define OUT_1()	(CurrGPIO->BSRR = 1 << curr_pin)
#define OUT_0()	(CurrGPIO->BSRR = 1 << (curr_pin + 16))

#define SET_IN()	(CurrGPIO->MODER &= ~(1 << curr_pin*2))

static void InitGPIO_RCC(GPIO_TypeDef *GPIO) {
	if (GPIO == GPIOA) {
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	}
	else if (GPIO == GPIOB) {
		RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	}
	else if (GPIO == GPIOC) {
		RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	}
	else if (GPIO == GPIOD) {
		RCC->AHBENR |= RCC_AHBENR_GPIODEN;
	}
}

/*	tick
 * 	1 - 1.5 uS
 * 	10 - 4 uS
 * 	20 - 7 uS
 * 	30 - 9.8 uS
 * 	50
 * 	100 - 30 uS
 *	150 - 44.7 uS
 *	200	- 59.1 uS
 *	300 - 88.2 uS
 *	340 - 99.8
 *	343 - 100.6
 *	350 - 102.7 uS
 *	400 - 117.2 uS
 *	500 - 146.1 uS
 *	688 - 200.1 uS
 * 	1000 - 291.2 uS
 * 	1030 - 300 uS
 * 	1700 - 494.3
 * 	1720 - 500 uS
 *	2000 - 581.3 uS
 */
void Delay_tick(uint16_t tick) {
//	uint32_t tick = (float)us / (float)((1000000 / (float)SystemCoreClock));
	for (uint32_t i = 0; i < tick; i++);
}

static uint8_t reset_ds18() {
	SET_OUT();	//write
	OUT_0();
	Delay_tick(1700);	// > 500 uS
	SET_IN();	//read...
	Delay_tick(300);	//88 uS
	if ((CurrGPIO->IDR & (1 << curr_pin))) {
		SET_OUT();
		OUT_1();
		GPIOC->BSRR = GPIO_BSRR_BR_8;
		return DS18B20_ERR_NO_SENSOR;
	}
	GPIOC->BSRR = GPIO_BSRR_BS_8;
	Delay_tick(1030);	//300 uS

	SET_IN();
	return 0;
}

static void write_ds18(uint32_t data, uint8_t num_bit) {
	SET_OUT();

	for (uint8_t i = 0; i < num_bit; i++)
	{
		OUT_0();
		Delay_tick(1);	//approx 1 uS

		if ((data & (1 << i))) {
			OUT_1();
		}
		Delay_tick(300);	//88uS

		OUT_1();
		Delay_tick(4);
	}
	SET_IN();
}

static uint32_t read_ds18(uint8_t count_bit) {
	uint32_t data = 0;
	for (uint8_t i = 0; i < count_bit; i++) {
		SET_OUT();
		OUT_0();
		Delay_tick(10);
		SET_IN();
		Delay_tick(10);	//
		data |= ((CurrGPIO->IDR & (1 << curr_pin)) >> curr_pin) << i;
		Delay_tick(150);
	}

	SET_IN();
	return data;
}

void ds18b20_Init(GPIO_TypeDef *GPIO, uint16_t num_pin, uint8_t resolution) {
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;

	InitGPIO_RCC(GPIO);
	CurrGPIO = GPIO;
	curr_pin = num_pin;

	SET_OUT();
	reset_ds18();
	ds18b20_SkipRom();

//	ds18b20_SetResolution(resolution);
}

void ds18b20_SetResolution(uint8_t resolution) {
	uint32_t th_th_resolution = 0;
//	write_ds18(0x44, 8);
}

uint32_t ds18b20_GetTemperature(){
	uint32_t temperature = 0;

	reset_ds18();

	ds18b20_SkipRom();	//0xCC

	Delay_tick(30);	//10uS

	write_ds18(0xBE, 8);	//read scratchpad
	Delay_tick(2);	//2uS
	temperature = read_ds18(16);	//read data from device....
	return temperature;
}

void ds18b20_StartMeas() {
	reset_ds18();

	ds18b20_SkipRom();
	Delay_tick(30);	//10 uS

	write_ds18(0x44, 8);	//convert_T init single temperature conversion
}

uint64_t ds18b20_GetROMCode() {
	return 0;
}

void ds18b20_SetTh(uint16_t th) {

}

void ds18b20_SetTl(uint16_t tl) {

}

void ds18b20_SkipRom() {
	write_ds18(0xCC, 8);
}

uint8_t ds18b20_isConversion() {
	SET_IN();
	Delay_tick(100);
	if (CurrGPIO->IDR & (1 << curr_pin)) {
		return 1;	//conversion is done
	}

	return 0;
}



