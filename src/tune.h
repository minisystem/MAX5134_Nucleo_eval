/*
 * tune.h
 *
 *  Created on: Jul 7, 2017
 *      Author: jeff
 */

#ifndef TUNE_H_
#define TUNE_H_

#include "stm32f4xx_it.h"

uint16_t interpolate_pitch_cv(uint8_t note, uint16_t *pitch_table);




#endif /* TUNE_H_ */
