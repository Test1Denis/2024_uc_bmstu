/*
 * ds18b20.h
 *
 *  Created on: 14 дек. 2020 г.
 *      Author: Tr
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include <stm32f0xx.h>

#define	DS18B20_RES_9	0x1F		//93,75 ms	0.5C
#define	DS18B20_RES_10	(0x1F | 1 << 5)		//187.5 ms	0.25C
#define	DS18B20_RES_11	(0x1F | 1 << 6)		//375 ms	0.125C
#define	DS18B20_RES_12	(0x1F | (1 << 5) | (1 << 6))		//750 ms	0.0625C

#define	DS18B20_ERR_OK	0
#define	DS18B20_ERR_NO_SENSOR	1

void Delay_tick(uint16_t us);

void ds18b20_Init(GPIO_TypeDef *GPIO, uint16_t num_pin, uint8_t resolution);
void ds18b20_SetResolution(uint8_t resolution);
uint32_t ds18b20_GetTemperature();
void ds18b20_StartMeas();

uint64_t ds18b20_GetROMCode();

void ds18b20_SkipRom();
void ds18b20_SetTh(uint16_t th);
void ds18b20_SetTl(uint16_t tl);
uint8_t ds18b20_isConversion();



#endif /* DS18B20_H_ */
