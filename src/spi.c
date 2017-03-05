/*
 * spi.c
 *
 *  Created on: Mar 3, 2017
 *      Author: jeff
 */

#include "stm32f4xx_it.h"
#include <stdlib.h>

//forward declarations
void init_spi(void);
void spi_write_dac(uint16_t value, uint8_t channel);

void init_spi(void) {

	//initialize MOSI and SCK pins
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);

	GPIO_InitTypeDef gpio_init; //GPIOA clock set in leds.c
	gpio_init.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5; //PA7 = MOSI, PA5 = SCK
	gpio_init.GPIO_Speed = GPIO_Fast_Speed;
	gpio_init.GPIO_Mode = GPIO_Mode_AF;
	gpio_init.GPIO_PuPd = GPIO_PuPd_UP; //enable pull up?
	GPIO_Init(GPIOA, &gpio_init);

	//initialize LDAC and CS/SS pin - this should've been PA4 for automatic hardware SS, but we used PA8 instead, duh.
	gpio_init.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //PA8 = CS/SS, PA9 = LDAC
	gpio_init.GPIO_Speed = GPIO_Fast_Speed;
	gpio_init.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &gpio_init);

	//initialize SPI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); //enable SPI clock

	SPI_InitTypeDef spi_init;
	spi_init.SPI_Direction = SPI_Direction_1Line_Tx;
	spi_init.SPI_Mode = SPI_Mode_Master;
	spi_init.SPI_DataSize = SPI_DataSize_8b; //8b? Need to clock in 24 bits of data per DAC channel
	spi_init.SPI_CPOL = SPI_CPOL_Low; //5134 uses low to high and high to low clock transitions. ie. idle state is LOW
	spi_init.SPI_CPHA = SPI_CPHA_2Edge; //clock phase - data is clocked on falling edge of clock pulse
	spi_init.SPI_NSS = SPI_NSS_Soft; //DAC chip select is handled in software
	spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //this should be APB2 clock/2, so 25 MHz SPI clock speed?
	spi_init.SPI_FirstBit = SPI_FirstBit_MSB; //check datasheet
	spi_init.SPI_CRCPolynomial = 7; //what is this?
	SPI_Init(SPI1, &spi_init);
	SPI_Cmd(SPI1, ENABLE);

	GPIO_SetBits(GPIOA, GPIO_Pin_8); //set DAC CS/SS HIGH
	GPIO_SetBits(GPIOA, GPIO_Pin_9); //set LDAC high

	//initialize DMA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //SPI1 TX is Channel 3, Stream 3 of DMA2
	DMA_DeInit(DMA2_Stream3); //SPI1 TX is DMA2 Stream 3
	DMA_InitTypeDef dma_init;
	DMA_StructInit(&dma_init); //set all values to default
	dma_init.DMA_PeripheralBaseAddr = (uint32_t) (&(SPI1->DR));
	dma_init.DMA_Channel = DMA_Channel_3; //SPI1 TX is on DMA Channel 3
	dma_init.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//dma_init.DMA_Memory0BaseAddr = (uint32_t)TX_buffer; //need to declare TX_buffer in main. It should be array of 3 uint8_t (24 bits)
	DMA_Init(DMA2_Stream3, &dma_init);

	//initialize DMA interrupt controller
	NVIC_InitTypeDef nvic_init;
	nvic_init.NVIC_IRQChannel = DMA2_Stream3_IRQn;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_init.NVIC_IRQChannelSubPriority = 1;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
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


