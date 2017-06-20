//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// -------------------
//#define NDEBUG
#include "main.h"
#include "stm32f4xx_it.h"
#include <stdlib.h>

#include <leds.h>
#include <stdio.h>
#include <diag/Trace.h>

#include "Timer.h"
#include "hardware.h"
#include "switches.h"
#include "midi.h"
#include "adc.h"
#include "spi.h"
#include "system.h"

#include "xnormidi-develop/midi.h"
#include "xnormidi-develop/midi_device.h"

//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

MidiDevice midi_device;

int
main(int argc, char* argv[])
{
	// Send a greeting to the trace device (skipped on Release).
  trace_puts("Hello this is STM32F410RB Nucelo!");

  //set up system clock
  setup_system_clock();

  //send clock frequency to console
  trace_printf("System clock: %u MHz\n", SystemCoreClock/1000000);



  timer_start(100); //set systick @2KHz


  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);

  init_spi();
  //init LEDs
  gpio_init_output(GPIOA, MIDI_LED);
  gpio_init_output(GATE_LED_PORT, GATE_LED_1);
  gpio_init_output(GATE_LED_PORT, GATE_LED_2);
  gpio_init_output(GATE_LED_PORT, GATE_LED_3);
  gpio_init_output(GATE_LED_PORT, GATE_LED_4);

  //GATE_LED_PORT->ODR |= 0xF000;

  //initialize_switches
  gpio_init_switch(CH_SW_PORT, CH1_SW);
  gpio_init_switch(CH_SW_PORT, CH2_SW);
  gpio_init_switch(CH_SW_PORT, CH3_SW);
  gpio_init_switch(CH_SW_PORT, CH4_SW);

  init_midi_usart();


  init_sine_lut();

  midi_device_init(&midi_device);
  //register callbacks
  midi_register_noteon_callback(&midi_device, note_on_event);
  midi_register_noteoff_callback(&midi_device, note_off_event);
  midi_register_realtime_callback(&midi_device, real_time_event);

  init_adc();

  init_channels();
  flag.sys_tick = 0;

  //just some initial data for testing
  channel[0].cv = CODE_OFFSET;
  channel[1].cv = CODE_OFFSET + CODE_INTERVAL;
  channel[2].cv = CODE_OFFSET + 2*CODE_INTERVAL;
  channel[3].cv = CODE_OFFSET + 3*CODE_INTERVAL;

  channel[0].octave_index = 0;

  // Infinite loop
  while (1)
    {


	  if (flag.sys_tick) {
		flag.sys_tick = 0;
		uint8_t current_state = 0;
		//read channel switches
		for (int i = 0; i < NUM_CHANNELS; i++) {
			current_state = !(GPIO_ReadInputDataBit(button[i].port, button[i].pin)); //active LOW switch
			current_state ^= button[i].state;
			button[i].state ^= current_state;
			current_state &= button[i].state;

			if (current_state) {
				GATE_LED_PORT->ODR &= GATE_LED_MASK;
				GATE_LED_PORT->ODR |= (1 << (i+12));
				channel[i].selected = 1;
				current_channel = i;
				//GPIO_SetBits(GATE_LED_PORT, channel[i].led) ;
			} else {
				channel[i].selected = 0;
				//GPIO_ResetBits(GATE_LED_PORT, channel[i].led);
			}
		}
		//read function switches
		current_state = !(GPIO_ReadInputDataBit(button[REC_SW_INDEX].port, button[REC_SW_INDEX].pin)); //active LOW switch
		current_state ^= button[REC_SW_INDEX].state;
		button[REC_SW_INDEX].state ^= current_state;
		current_state &= button[REC_SW_INDEX].state;
		if (current_state) {
			//in calibration mode write octave code
			//button[REC_SW_INDEX].state ^= (1<<0);
			GPIO_ToggleBits(GPIOA, MIDI_LED);
			//clumsy mode toggling
			if (mode == NORMAL) {
				mode = CALIBRATE;
			} else {
				mode = NORMAL;
			}
			//if (channel[0].octave_index++ > NUM_OCTAVES) channel[0].octave_index = 0;
			//GPIO_SetBits(GATE_LED_PORT, GATE_LED_4);

		} else {
			//GPIO_ResetBits(GATE_LED_PORT, GATE_LED_4);
		}

		//handle ADC stuff
		int adc_change = adc_buffer[0] - adc_new_value[0];
		adc_new_value[0] = adc_new_value[0] + (adc_change >> 4);
			//adc_new_value = adc_buffer;
			//trace_printf("ADC VALUE: %u\n", adc_buffer);
		//}
		adc_change = adc_buffer[1] - adc_new_value[1];
		adc_new_value[1] = adc_new_value[1] + (adc_change >> 4);
		if (adc_new_value[1] > 2048) {

			//GPIO_SetBits(GPIOA, MIDI_LED);
		} else {
			//GPIO_ResetBits(GPIOA, MIDI_LED);
		}
		uint16_t DAC_value = 0;
		for (int i = 0; i < NUM_OCTAVES; i++) {

			if (((adc_new_value[0]*10) < (POT_INTERVAL*i + POT_INTERVAL)) && ((adc_new_value[0]*10) > POT_INTERVAL*(i))) { //11 intervals from -3V to +7V
				DAC_value = channel[0].pitch_table[i];
				channel[0].octave_index = i;
				//DAC_value = 3235 + 4750*i;
			}
		}

//		if (adc_new_value[0] < 409) { //this is a stupid exception to handle noise on the ADC channel - bleh!
//			DAC_value = channel[0].pitch_table[0];
//			channel[0].octave_index = 0;
//		}

		if (mode == CALIBRATE) {
			if (adc_new_value[1] > 2048) {

				//need to check for overflow - havne't done that yet
				channel[current_channel].pitch_table[channel[0].octave_index] += (adc_new_value[1] - 2048) >> 1;

			} else {
				//need to check for overflow - haven't done that yet
				channel[current_channel].pitch_table[channel[0].octave_index] -= (2048 - adc_new_value[1]) >> 1;

			}

		}

		//button[CH1_SW_INDEX].state ^= current_state;
		//current_state ^= button[CH1_SW_INDEX].state;

//		if (current_state) {
//
//			//button[CH1_SW_INDEX].state ^= current_state;
//			GPIO_ToggleBits(GATE_LED_PORT, GATE_LED_1);
//			//GPIO_SetBits(GATE_LED_PORT, GATE_LED_1);
//		}
	  }


    }
  // Infinite loop, never return.
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
