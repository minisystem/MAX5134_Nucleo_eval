/*
 * adc.c
 *
 *  Created on: Mar 10, 2017
 *      Author: jeff
 */



#include "stm32f4xx_it.h"
#include "hardware.h"
#include <stdlib.h>

__IO uint16_t adc_buffer = 0;//ADC1->DR;
__IO uint16_t adc_new_value = 0;

void init_adc(void) {

	//ADC1 channel 11
	//ADC1 is DMA Stream 0, Channel 0

	//GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_Mode_AF);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Pin = GPIO_Pin_1; //ADC input is ADC1
	gpio_init.GPIO_Speed = GPIO_Fast_Speed;
	gpio_init.GPIO_Mode = GPIO_Mode_AN;
	//gpio_init.GPIO_OType = GPIO_OType_PP; //not sure which type is needed
	gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &gpio_init);

	//ADC Common Init
	ADC_CommonInitTypeDef adc_common_init;
	adc_common_init.ADC_Mode = ADC_Mode_Independent;
	adc_common_init.ADC_Prescaler = ADC_Prescaler_Div8; //what speed?
	adc_common_init.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	adc_common_init.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&adc_common_init);

	//ADC1 init
	ADC_InitTypeDef adc_init;
	ADC_DeInit();
	adc_init.ADC_Resolution = ADC_Resolution_12b;
	adc_init.ADC_ScanConvMode = DISABLE; //scan just one channel
	adc_init.ADC_NbrOfConversion = 1; //
	adc_init.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc_init.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init.ADC_ContinuousConvMode = ENABLE;
	ADC_Init(ADC1, &adc_init);

	//DMA init ADC1 is DMA2 Stream 0, Channel 0
	DMA_InitTypeDef DMA_InitStructure;
	//DMA2 clock is already running from USART and SPI initialization
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_144Cycles);
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);

}
