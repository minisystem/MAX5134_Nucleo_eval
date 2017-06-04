/*
 * adc.h
 *
 *  Created on: Mar 10, 2017
 *      Author: jeff
 */

#ifndef ADC_H_
#define ADC_H_


#include "stm32f4xx.h"

extern __IO uint16_t adc_buffer[2];
extern __IO uint16_t adc_new_value[2];

void init_adc(void);


#endif /* ADC_H_ */
