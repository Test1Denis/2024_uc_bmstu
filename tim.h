/*
 * tim.h
 *
 *  Created on: Apr 11, 2024
 *      Author: protuberanec
 */

#ifndef TIM_H_
#define TIM_H_

#include <stm32f0xx.h>

void tim6_init();
void tim6_start();
void tim6_stop();

void tim3_init_as_pwm();


#endif /* TIM_H_ */
