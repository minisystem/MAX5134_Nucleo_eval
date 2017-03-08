/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"
#include "Timer.h"
#include "leds.h"
#include "hardware.h"
#include "diag/Trace.h"
#include "midi.h"
#include <math.h>
//#include "xnormidi-develop/midi.h"
//#include "midi_device.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static uint16_t sine_lut[256];
void init_sine_lut(void);

void init_sine_lut(void) {
	for (int i = 0; i < 256; i++) {
		sine_lut[i]=(cos((i/255.0+1.0)*3.14)+1.0)/2.0*65535.0;
	}
}

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void) //currently executes every 1ms
{
  //TimingDelay_Decrement();
	//timer_tick ();
	//turn_led_off(GPIOA, LED1);
	//turn_led_off(GPIOB, LED2);
	//turn_led_off(GPIOC, LED3);
	//turn_led_off(GPIOA, LED4);
	midi_device_process(&midi_device); //this needs to be called 'frequently' in order for MIDI to work
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

void USART2_IRQHandler(void) {

	uint8_t data;
	if (USART_GetITStatus(USART2,USART_IT_RXNE)){

		data = USART_ReceiveData(USART2) & 0xFF;
		turn_led_on(GPIOC, LED3);
		midi_device_input(&midi_device, 1, &data);
		turn_led_off(GPIOC, LED3);
		//trace_printf("MIDI DATA: %u\n", data);
		turn_led_on(GPIOB, LED2);
		turn_led_off(GPIOA, LED1);
	}

	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	//NVIC_ClearPendingIRQ(USART2_IRQn);

}

void DMA2_Stream3_IRQHandler(void) { //SPI1 DMA IRQ Handler
	//turn_led_off(GPIOA, LED1);
	//turn_led_on(GPIOA, LED4);
	//http://www.micromouseonline.com/2012/03/11/adding-dma-to-the-spi-driver-with-the-stm32f4/
	uint8_t DAC_ctrl_byte[DAC_CHAN_NUM] = {

			DAC_CHAN_0,
			DAC_CHAN_1,
			DAC_CHAN_2,
			DAC_CHAN_3

	};

	if (DMA_GetITStatus(DMA2_Stream3, DMA_IT_TCIF3)) { //test if DMA Stream transfer complete (why? Isn't that the point of the interrupt being called?)
		dac_update_flag = 1;
		DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_TCIF3); //clear interrupt DMA IRQ flag bit
		DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3); //isn't this the same as line above?
		//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}; //wait for data to be flushed from transmit buffer
		//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET){}; //wait for data to be flushed from shift register
		GPIO_SetBits(GPIOA, DAC_CS_PIN); //release DAC

		uint16_t data_port_mask = DATA_PORT->ODR & 0xFFFC; //clear bottom 3 bits
		uint16_t data_out = data_port_mask | DAC_index;
		DATA_PORT->ODR = data_out;
		GPIO_ResetBits(DAC_MUX_PORT, DAC_MUX_1);

		TX_buffer[0] = DAC_ctrl_byte[DAC_index];

		if (DAC_index++ >= DAC_CHAN_NUM) { //finished sending data to all 4 DAC channels
			GPIO_ResetBits(GPIOA, LDAC_PIN); //pulse LDAC to update DAC registers
			DAC_index = 0;
			DAC_counter+= 4;// = DAC_counter +4; //increment dac_value
			phase_accumulator += 1024;
			//TX_buffer[1] = DAC_counter; //set top byte
			TX_buffer[0] = 0; //this is the magic right here for some reason - not setting this causes incoming MIDI USART messages to screw up DAC updating
			//WHY? WHY GODDAMMIT WHY? //this control byte is 'No operation'
			TX_buffer[1] = sine_lut[phase_accumulator >>8] >> 8;
			TX_buffer[2] = sine_lut[phase_accumulator >>8] & 0xFF; //set bottom byte
			GPIO_SetBits(DAC_MUX_PORT, DAC_MUX_1);

		}
		//TX_buffer[0] = DAC_ctrl_byte[DAC_index];

		//if (TX_buffer[1] == 1) turn_led_on(GPIOA, LED4); //see if there's any asynchronous zeroing of TX_buffer

		GPIO_SetBits(GPIOA, LDAC_PIN); //set LDAC PIN
		GPIO_ResetBits(GPIOA, DAC_CS_PIN); //select DAC

		//now initiate next DMA transfer
		//don't really need to reset these - these are configured in spi setup function
		//DMA2_Stream3->NDTR = (uint32_t)TX_BUFFER_SIZE;
		//DMA2_Stream3->M0AR = (uint32_t)TX_buffer;
		DMA_Cmd(DMA2_Stream3, ENABLE); //need to re-enable DMA transfer as it turns off itself once transfer is complete
		//DMA_ITConfig(DMA2_Stream3, DMA_IT_TC, ENABLE);
	}
	NVIC_ClearPendingIRQ(DMA2_Stream3_IRQn);
		//turn_led_off(GPIOA, LED4);

}

void DMA1_Stream5_IRQHandler(void) {
	NVIC_ClearPendingIRQ(DMA1_Stream5_IRQn); //need to manually clear to minimize interference with SPI DMA
	//while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE));
	if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5)) {
		//turn_led_on(GPIOA, LED4);
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
		DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
		midi_device_input(&midi_device, 1, &midi_dma_buffer);
		//trace_printf("MIDI DATA: %u\n", midi_dma_buffer);
		//DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
		//DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5 | DMA_FLAG_HTIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_FEIF5);
		//DMA1_Stream5->NDTR = (uint32_t) 1;

		//DMA_Cmd(DMA1_Stream5, ENABLE);
	}

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
