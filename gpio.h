/*
 * gpio.h
 *
 *  Created on: Apr 4, 2024
 *      Author: protuberanec
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f0xx.h"

void gpioB_init();


void gpioC_init();
void gpioC_init_for_pwm();
void gpioC_setBit(int num_bit);
void gpioC_resetBit(int num_bit);
void gpioC_toggleBit(int numb_bit);

void gpioA_init();
int gpioA_getIDR();
int getIDR(GPIO_TypeDef* gpio);




#endif /* GPIO_H_ */
