//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef BLINKLED_H_
#define BLINKLED_H_

#include "main.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "hardware.h"

// ----- LED definitions ------------------------------------------------------

// Adjust these definitions for your own board.

#if defined(BOARD_OLIMEX_STM32_E407)

// STM32-E407 definitions (the GREEN led, C13, active low)

// Port numbers: 0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, ...
#define BLINK_PORT_NUMBER               LED1_PORT
#define BLINK_PIN_NUMBER                (13)
#define BLINK_ACTIVE_LOW                (1)

#else

// USING STM F410RB from a NUCLEO 64 board. We found this number (port A5)
// on page 23 of this document:
// http://www.st.com/content/ccc/resource/technical/document/user_manual/98/2e/fa/4b/e0/82/43/b7/DM00105823.pdf/files/DM00105823.pdf/jcr:content/translations/en.DM00105823.pdf

#define BLINK_PORT_NUMBER               LED1_PORT
#define BLINK_PIN_NUMBER                (4)
#define BLINK_ACTIVE_LOW                (0)

#endif

#define GPIOx(_N)                 ((GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*(_N)))
#define PIN_MASK(_N)              (1 << (_N))
#define RCC_MASKx(_N)             (RCC_AHB1ENR_GPIOAEN << (_N))

// ----------------------------------------------------------------------------

struct led {

	GPIO_TypeDef *port;
	uint32_t pin;
};


extern
void
blink_led_init(GPIO_TypeDef * port, uint32_t led_port, uint32_t led_pin);

//void turn_led_on(struct led led);
//void turn_led_off(struct led led);

void turn_led_on(GPIO_TypeDef *port, uint32_t led_pin);
void turn_led_off(GPIO_TypeDef *port, uint32_t led_pin);
void turn_off_all_leds(void);

// ----------------------------------------------------------------------------
//none of the below is used anymore
inline void
blink_led_on(void);

inline void
blink_led_off(void);

// ----------------------------------------------------------------------------

inline void
__attribute__((always_inline))
blink_led_on(void)
{/*
#if (BLINK_ACTIVE_LOW)
    HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER),
        BLINK_PIN_MASK(BLINK_PIN_NUMBER), GPIO_PIN_RESET);
#else
    HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER),
        BLINK_PIN_MASK(BLINK_PIN_NUMBER), GPIO_PIN_SET);
#endif
*/
}

inline void
__attribute__((always_inline))
blink_led_off(void)
{
	/*
#if (BLINK_ACTIVE_LOW)
    HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER),
        BLINK_PIN_MASK(BLINK_PIN_NUMBER), GPIO_PIN_SET);
#else
    HAL_GPIO_WritePin(BLINK_GPIOx(BLINK_PORT_NUMBER),
        BLINK_PIN_MASK(BLINK_PIN_NUMBER), GPIO_PIN_RESET);
#endif
*/
}

// ----------------------------------------------------------------------------

#endif // BLINKLED_H_
