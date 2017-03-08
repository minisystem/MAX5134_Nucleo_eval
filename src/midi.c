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
#include "xnormidi-develop/midi.h"
#include "xnormidi-develop/midi_device.h"

__IO uint8_t midi_dma_buffer = 0;

void init_midi_usart(void) {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //enable peripheral clock


	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //is this necessary? Alternative function for this pin is set below. YES IT IS.
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
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	NVIC_InitTypeDef NVIC_InitStruct;
//	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
//	NVIC_Init(&NVIC_InitStruct);

	//OK let's try DMA mode

	//initialize DMA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); //USART2 is DMA1, Stream5, Channel 4
	DMA_DeInit(DMA1_Stream5); //USART2 is DMA1 stream 5
	DMA_InitTypeDef dma_init;
	DMA_StructInit(&dma_init); //set all values to default
	dma_init.DMA_BufferSize = 1; //just one byte - would be nice to grab 3 bytes before generating interrupt, but some messages are only 1 or 2 bytes
	dma_init.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR));
	dma_init.DMA_Channel = DMA_Channel_4; //USART2 RX is on DMA1 Channel 4
	dma_init.DMA_DIR = DMA_DIR_PeripheralToMemory;
	dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_init.DMA_Memory0BaseAddr = (uint32_t)&midi_dma_buffer;
	dma_init.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_init.DMA_Priority = DMA_Priority_High;
	dma_init.DMA_FIFOMode = DMA_FIFOMode_Disable ;
	dma_init.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
	dma_init.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
	dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_init.DMA_Mode = DMA_Mode_Circular;
	DMA_Init(DMA1_Stream5, &dma_init);

	DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE); //enable DMA transfer complete interrupt

	//initialize DMA USART TC interrupt

	NVIC_InitTypeDef nvic_init;
	nvic_init.NVIC_IRQChannel = DMA1_Stream5_IRQn;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_init.NVIC_IRQChannelSubPriority = 1;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init);
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);

	DMA_Cmd(DMA1_Stream5,ENABLE);
}

void note_on_event(MidiDevice * device, uint8_t channel, uint8_t note, uint8_t velocity) {

	turn_led_on(GPIOA, LED1);

}
void note_off_event(MidiDevice * device, uint8_t status, uint8_t note, uint8_t velocity) {

	turn_led_off(GPIOA, LED1);
}
void real_time_event(MidiDevice * device, uint8_t real_time_byte) {

	//turn_led_on(GPIOA, LED4);
	GPIO_ToggleBits(GPIOA, GPIO_Pin_10);

}
