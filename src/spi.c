/*
 * spi.c
 *
 *  Created on: Mar 3, 2017
 *      Author: jeff
 */

#include "stm32f4xx_it.h"
#include "hardware.h"
#include "spi.h"
#include <stdlib.h>

__IO uint8_t TX_buffer[TX_BUFFER_SIZE];
__IO uint8_t dac_word[TX_BUFFER_SIZE];
__IO uint8_t DAC_index = 0;
__IO uint8_t DAC_counter = 0;
__IO uint16_t phase_accumulator = 0;
__IO uint8_t dac_mux_addr = 0;
__IO uint8_t dac_update_flag = 0;



void init_spi(void) {

	//initialize MOSI and SCK pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef gpio_init; //GPIOA clock started in leds.c

	gpio_init.GPIO_Pin = GPIO_Pin_10; //PA10 = MOSI
	gpio_init.GPIO_Speed = GPIO_Fast_Speed;
	gpio_init.GPIO_Mode = GPIO_Mode_AF;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_UP; //enable pull up?
	GPIO_Init(GPIOA, &gpio_init);


	gpio_init.GPIO_Pin = GPIO_Pin_0; //PB0 = SCK
	gpio_init.GPIO_Speed = GPIO_Fast_Speed;
	gpio_init.GPIO_Mode = GPIO_Mode_AF;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_UP; //enable pull up?
	GPIO_Init(GPIOB, &gpio_init);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF6_SPI5);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF6_SPI5);

	//initialize LDAC and CS/SS pin - this should've been PA4 for automatic hardware SS, but we used PA8 instead, duh.
	gpio_init.GPIO_Pin = DAC_CS_PIN | LDAC_PIN; //PA4 = CS/SS, PA7 = LDAC
	gpio_init.GPIO_Speed = GPIO_Fast_Speed;
	gpio_init.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &gpio_init);




	//initialize SPI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE); //enable SPI clock

	SPI_InitTypeDef spi_init;
	spi_init.SPI_Direction = SPI_Direction_1Line_Tx;
	spi_init.SPI_Mode = SPI_Mode_Master;
	spi_init.SPI_DataSize = SPI_DataSize_8b; //8b? Need to clock in 24 bits of data per DAC channel
	spi_init.SPI_CPOL = SPI_CPOL_Low; //5134 uses low to high and high to low clock transitions. ie. idle state is LOW
	spi_init.SPI_CPHA = SPI_CPHA_2Edge; //clock phase - data is clocked on falling edge of clock pulse
	spi_init.SPI_NSS = SPI_NSS_Soft; //DAC chip select is handled in software
	spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //APB2 clock/2, so 25 MHz SPI clock speed
	spi_init.SPI_FirstBit = SPI_FirstBit_MSB; //check datasheet
	spi_init.SPI_CRCPolynomial = 7; //what is this?
	SPI_Init(SPI5, &spi_init);
	SPI_Cmd(SPI5, ENABLE);

	GPIO_SetBits(GPIOA, DAC_CS_PIN); //set DAC CS/SS HIGH
	GPIO_SetBits(GPIOA, LDAC_PIN); //set LDAC high

	//initialize DMA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //SPI5 TX is Channel 2, Stream 4 of DMA2
	DMA_DeInit(DMA2_Stream4); //SPI1 TX is DMA2 Stream 3
	DMA_InitTypeDef dma_init;
	DMA_StructInit(&dma_init); //set all values to default
	dma_init.DMA_BufferSize = TX_BUFFER_SIZE;
	dma_init.DMA_PeripheralBaseAddr = (uint32_t) (&(SPI5->DR));
	dma_init.DMA_Channel = DMA_Channel_2; //SPI5 TX is on DMA Channel 2
	dma_init.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_init.DMA_Memory0BaseAddr = (uint32_t)TX_buffer; //TX_buffer is array of 3 uint8_t (24 bits)
	dma_init.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_init.DMA_Priority = DMA_Priority_High;
	dma_init.DMA_FIFOMode = DMA_FIFOMode_Disable ;
	dma_init.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
	dma_init.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
	dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_init.DMA_Mode = DMA_Mode_Normal;
	//DMA_DoubleBufferModeConfig(DMA2_Stream3, (uint32_t)dac_word, DMA_Memory_0);
	//DMA_DoubleBufferModeCmd(DMA2_Stream3, ENABLE);
	DMA_Init(DMA2_Stream4, &dma_init);
	DMA_ITConfig(DMA2_Stream4, DMA_IT_TC, ENABLE); //enable DMA transfer complete interrupt

	//initialize DMA interrupt controller
	NVIC_InitTypeDef nvic_init;
	nvic_init.NVIC_IRQChannel = DMA2_Stream4_IRQn;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_init.NVIC_IRQChannelSubPriority = 0;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init);
	SPI_I2S_DMACmd(SPI5, SPI_I2S_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA2_Stream4, ENABLE); //enable DMA

}

void spi_write_dac(uint16_t value, uint8_t channel) { //currently just use busy/wait to transmit data to test DAC

	uint8_t dac_low = value & 0xFF; //take bottom 8 bits
	uint8_t dac_high = value >> 8; //take top 8 bits
	GPIO_ResetBits(GPIOA, GPIO_Pin_8); //CS low
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//wait for empty buffer
	//maybe switch to 16 bit transmission here? Once DMA is implemented would it be faster?
	SPI_I2S_SendData(SPI1, channel); //send control byte
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET); //wait for byte to be sent
	SPI_I2S_SendData(SPI1, dac_high); //send first data byte
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET); //wait for byte to be sent
	SPI_I2S_SendData(SPI1, dac_low); //send second data byte
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET); //wait for byte to be sent
	GPIO_SetBits(GPIOA, GPIO_Pin_8);


}

void spi_dma_write(uint16_t value, uint8_t channel) {

	TX_buffer[0] = channel;
	TX_buffer[1] = value >> 8; //top high byteRCCs
	TX_buffer[2] = value & 0xFF; //low byte
	GPIO_ResetBits(GPIOA, DAC_CS_PIN); //select DAC
	DMA_Cmd(DMA2_Stream4, ENABLE); //enable DMA
	SPI_I2S_DMACmd(SPI5, SPI_I2S_DMAReq_Tx, ENABLE); //enable SPI1 TX request
	while (DMA_GetFlagStatus(DMA2_Stream4, DMA_FLAG_TCIF4) == RESET); //wait for transfer to complete
	GPIO_SetBits(GPIOA, DAC_CS_PIN); //release DAC
	DMA_ClearFlag(DMA2_Stream4, DMA_FLAG_TCIF4);

}


