/*
 * spi.h
 *
 *  Created on: May 29, 2017
 *      Author: jeff
 */

#ifndef SPI_H_
#define SPI_H_

void init_spi(void);
void spi_write_dac(uint16_t value, uint8_t channel);
void spi_dma_write(uint16_t value, uint8_t channel);

#endif /* SPI_H_ */
