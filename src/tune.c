/*
 * tune.c
 *
 *  Created on: Jul 7, 2017
 *      Author: jeff
 */

#include "tune.h"

uint16_t interpolate_pitch_cv(uint8_t note, uint16_t *pitch_table) {

	uint8_t pitch_index = note/12;
	uint8_t delta_note = note - pitch_index*12;
	uint16_t y0 = pitch_table[pitch_index - 1];
	uint16_t y1 = pitch_table[pitch_index];

	uint16_t interpolated_pitch_cv = y0 + (((y1 - y0)*delta_note)/12);

	return interpolated_pitch_cv;
}
