/*
 * midi.c
 *
 *  Created on: Feb 19, 2017
 *      Author: jeff
 */

#include "midi.h"
#include "hardware.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"


void init_midi_usart(void) {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);


	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	//initi USART RX PIN
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Pin = GPIO_Pin_3;// MIDI_RX_PIN;//(1 << MIDI_RX_PIN);
	gpio_init.GPIO_Speed = GPIO_Fast_Speed;
	gpio_init.GPIO_Mode = GPIO_Mode_AF; //GPIO_Mode_IN;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpio_init);

  // Initialize USART.
  USART_InitTypeDef usart_init;
  usart_init.USART_BaudRate = 31250;
  usart_init.USART_WordLength = USART_WordLength_8b;
  usart_init.USART_StopBits = USART_StopBits_1;
  usart_init.USART_Parity = USART_Parity_No;
  usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  usart_init.USART_Mode = USART_Mode_Rx;

  USART_Init(USART2, &usart_init);
  USART_Cmd(USART2, ENABLE);

  //enable RX interrupt  Could maybe also use DMA Mode instead?
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  NVIC_InitTypeDef NVIC_InitStruct;
  NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&NVIC_InitStruct);

}

//void note_on_event(MidiDevice * device, uint8_t channel, uint8_t note, uint8_t velocity) {
//
//}
//void note_off_event(MidiDevice * device, uint8_t status, uint8_t note, uint8_t velocity) {
//
//}
//void real_time_event(MidiDevice * device, uint8_t real_time_byte) {
//
//}
