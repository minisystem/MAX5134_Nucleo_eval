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
  gpio_init_output(GPIOA, MIDI_LED);
  //GPIO_SetBits(GPIOA, MIDI_LED);


  //initialize_switches(SWITCH_PORT, SWITCH_PIN);
  init_midi_usart();


  init_sine_lut();

  midi_device_init(&midi_device);
  //register callbacks
  midi_register_noteon_callback(&midi_device, note_on_event);
  midi_register_noteoff_callback(&midi_device, note_off_event);
  midi_register_realtime_callback(&midi_device, real_time_event);

  init_adc();

  // Infinite loop
  while (1)
    {

//	  current_switch_state = read_switch(GPIOC, SWITCH_PIN);
//	  current_switch_state ^= previous_switch_state;
//	  previous_switch_state ^= current_switch_state;
//	  current_switch_state &= previous_switch_state;
//
//	  if (current_switch_state == 1) {
//		  current_switch_state ^= 1<<0;
//		  if (++led_index > 3) led_index = 0;
//	  }


//	  if (read_switch(GPIOC, SWITCH_PIN) == 1) {
//
//		  turn_led_on(GPIOA, LED1);
//	  } else {
//
//		  turn_led_off(GPIOA, LED1);
//	  }

    }
  // Infinite loop, never return.
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
