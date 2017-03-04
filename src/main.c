//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// -------------------
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

//#include "midi.h"
//#include "midi_device.h"

//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- Timing definitions -------------------------------------------------

// Set number of ticks for blink timer - fewer ticks for faster blinking
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ /2)*.25
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)*.25



// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

//MidiDevice midi_device;

int
main(int argc, char* argv[])
{
	// Send a greeting to the trace device (skipped on Release).
  trace_puts("Hello this is STM32F410RB Nucelo!");

  //set up system clock
  setup_system_clock();

  //send clock frequency to console
  trace_printf("System clock: %u MHz\n", SystemCoreClock/1000000);



  timer_start();

  blink_led_init(GPIOA, LED1_PORT, LED1);
  blink_led_init(GPIOB, LED2_PORT, LED2);
  blink_led_init(GPIOC, LED3_PORT, LED3);
  blink_led_init(GPIOA, LED4_PORT, LED4);
  
  initialize_switches(SWITCH_PORT, SWITCH_PIN);
  init_midi_usart();
  init_spi();
  //clear all DAC channels
  spi_write_dac(0, DAC_CHAN_0);
  spi_write_dac(0, DAC_CHAN_1);
  spi_write_dac(0, DAC_CHAN_2);
  spi_write_dac(0, DAC_CHAN_3);


  uint32_t current_switch_state = 0;
  uint32_t previous_switch_state = 0;
  uint8_t led_index = 0;
  uint32_t seconds = 0;
  struct led led[4] = {

  	{GPIOA, LED1},
  	{GPIOB, LED2},
  	{GPIOC, LED3},
  	{GPIOA, LED4}

  };

  spi_write_dac(0xFFF0, DAC_CHAN_0);
  // Infinite loop
  while (1)
    {

	  current_switch_state = read_switch(GPIOC, SWITCH_PIN);
	  current_switch_state ^= previous_switch_state;
	  previous_switch_state ^= current_switch_state;
	  current_switch_state &= previous_switch_state;

	  if (current_switch_state == 1) {
		  current_switch_state ^= 1<<0;
		  if (++led_index > 3) led_index = 0;
	  }

	  //turn_off_all_leds();
	  //turn_led_on(led[led_index].port, led[led_index].pin);
	  //turn_led_on(GPIOA, LED1);
//	  if (read_switch(GPIOC, SWITCH_PIN) == 1) {
//
//		  turn_led_on(GPIOA, LED1);
//	  } else {
//
//		  turn_led_off(GPIOA, LED1);
//	  }

	  //uint8_t blah;
	  turn_led_on(GPIOA, LED1);
//	  if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET) {
//		  turn_led_off(GPIOA, LED1);
//		  blah = USART2->DR & 0xff;
//		  trace_printf("MIDI DATA: %u\n", blah);
//	  }
//      for (int i = 1; i < 4; i++) {
//
//    	  turn_led_off(led[i-1].port, led[i-1].pin);
//    	  turn_led_on(led[i].port, led[i].pin);
//    	  timer_sleep(BLINK_OFF_TICKS);
//
//      }
//
//      for (int i = 3; i > 0; i--) {
//
//    	  turn_led_off(led[i].port, led[i].pin);
//    	  turn_led_on(led[i-1].port, led[i-1].pin);
//    	  timer_sleep(BLINK_OFF_TICKS);
//
//
//      }
      ++seconds;
      // Count seconds on the trace device.
      //trace_printf("Second %u\n", seconds);
      //printf("hey there\n");
    }
  // Infinite loop, never return.
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
