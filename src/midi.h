/*
 * midi.h
 *
 *  Created on: Feb 19, 2017
 *      Author: jeff
 */

#ifndef MIDI_H_
#define MIDI_H_

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include <inttypes.h>
#include "xnormidi-develop/midi.h"
#include "xnormidi-develop/midi_device.h"


extern MidiDevice midi_device;

void init_midi_usart(void);
void note_on_event(MidiDevice * device, uint8_t channel, uint8_t note, uint8_t velocity);
void note_off_event(MidiDevice * device, uint8_t status, uint8_t note, uint8_t velocity);
void real_time_event(MidiDevice * device, uint8_t real_time_byte);

#endif /* MIDI_H_ */
