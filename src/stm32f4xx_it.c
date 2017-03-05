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
//#include "xnormidi-develop/midi.h"
//#include "midi_device.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern __IO uint8_t TX_buffer[TX_BUFFER_SIZE];
extern __IO uint8_t TX_buffer_index;
extern __IO uint8_t DAC_index;
extern __IO uint8_t DAC_counter;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

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
void SysTick_Handler(void)
{
  //TimingDelay_Decrement();
	timer_tick ();
	turn_led_off(GPIOA, LED1);
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
		//midi_device_input(&midi_device, 1, &data);
		trace_printf("MIDI DATA: %u\n", data);
		turn_led_on(GPIOB, LED2);
		turn_led_off(GPIOA, LED1);
	}

	//USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	//NVIC_ClearPendingIRQ(USART2_IRQn);

}

void DMA2_Stream3_IRQHandler(void) { //SPI1 DMA IRQ Handler
	turn_led_off(GPIOA, LED1);
	turn_led_on(GPIOA, LED4);
	//http://www.micromouseonline.com/2012/03/11/adding-dma-to-the-spi-driver-with-the-stm32f4/
	uint8_t DAC_ctrl_byte[DAC_CHAN_NUM] = {

			DAC_CHAN_0,
			DAC_CHAN_1,
			DAC_CHAN_2,
			DAC_CHAN_3

	};

	if (DMA_GetITStatus(DMA2_Stream3, DMA_FLAG_TCIF3)) { //test if DMA Stream transfer complete (why? Isn't that the point of the interrupt being called?)

		DMA_ClearITPendingBit(DMA2_Stream3, DMA_FLAG_TCIF3); //clear interrupt DMA IRQ flag bit
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}; //wait for data to be flushed from transmit buffer
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET){}; //wait for data to be flushed from shift register


		//transfer is finished, now initiate next transfer
		//if (TX_buffer_index++ > TX_BUFFER_SIZE) { //24 bit transfer to DAC is complete
		//	TX_buffer_index = 0;
		GPIO_SetBits(GPIOA, DAC_CS_PIN); //release DAC
		if (DAC_index++ >= DAC_CHAN_NUM) { //finished sending data to all 4 DAC channels
			GPIO_ResetBits(GPIOA, LDAC_PIN); //pulse LDAC to update DAC registers
			DAC_index = 0;
			DAC_counter++; //increment dac_value
			TX_buffer[1] = DAC_counter >> 8; //set top byte
			TX_buffer[2] = DAC_counter & 0xFF; //set bottom byte
		}
		TX_buffer[0] = DAC_ctrl_byte[DAC_index];

		//}

		GPIO_SetBits(GPIOA, LDAC_PIN); //set LDAC PIN
		GPIO_ResetBits(GPIOA, DAC_CS_PIN); //select DAC

		//now initiate next DMA transfer
		DMA2_Stream3->NDTR = (uint32_t)TX_BUFFER_SIZE; //this value is decremented - does that mean data in TX_buffer needs to be reversed?
		DMA2_Stream3->M0AR = (uint32_t)TX_buffer;
		DMA_Cmd(DMA2_Stream3, ENABLE);
		DMA_ITConfig(DMA2_Stream3, DMA_IT_TC, ENABLE);
	}


}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
