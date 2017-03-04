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
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5; //PA7 = MOSI, PA5 = SCK
	gpio_init.GPIO_Speed = GPIO_Fast_Speed;
	gpio_init.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &gpio_init);

	//initialize CS/SS pin - this should've been PA4 for automatic hardware SS, but we used PA8 instead, duh.
	gpio_init.GPIO_Pin = GPIO_Pin_8;
	gpio_init.GPIO_Speed = GPIO_Fast_Speed;
	gpio_init.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &gpio_init);

	//initialize SPI
	SPI_InitTypeDef spi_init;
	spi_init.SPI_Direction = SPI_Direction_1Line_Tx;
	spi_init.SPI_Mode = SPI_Mode_Master;
	spi_init.SPI_DataSize = SPI_DataSize_16b;
	spi_init.SPI_CPOL = SPI_CPOL_High;
	spi_init.SPI_CPHA = SPI_CPHA_1Edge;





}

void spi_write_dac(uint16_t value, uint8_t channel) {


}


