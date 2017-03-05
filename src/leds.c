//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//
#include <leds.h>
// ----------------------------------------------------------------------------
//struct led led[4] = {
//
//	{GPIOA, LED1},
//	{GPIOB, LED2},
//	{GPIOC, LED3},
//	{GPIOA, LED4}
//
//};

void
blink_led_init(GPIO_TypeDef * port, uint32_t port_num, uint32_t led_pin)
{
  // Enable GPIO Peripheral clock
  //RCC->AHB1ENR |= BLINK_RCC_MASKx(BLINK_PORT_NUMBER);
  //RCC->AHB1ENR |= RCC_MASKx(led_port);

  RCC_AHB1PeriphClockCmd(port_num, ENABLE);


  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure pin in output push/pull mode
  //GPIO_InitStructure.Pin = BLINK_PIN_MASK(BLINK_PIN_NUMBER);
  GPIO_InitStructure.GPIO_Pin = (1 << led_pin);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  //int blink_gpio = BLINK_GPIOx(BLINK_PORT_NUMBER);
  GPIO_Init(port, &GPIO_InitStructure);

  // Start with led turned off
  //blink_led_off();
}

void turn_led_on(GPIO_TypeDef *port, uint32_t led_pin) {

	port->BSRRL = (1<<led_pin);
}

void turn_led_off(GPIO_TypeDef *port, uint32_t led_pin) {

	port->BSRRH = ((1<<led_pin));

}

void turn_off_all_leds(void) {

	/*GPIOA->BSRR = (1<<LED1) << 16;
	GPIOB->BSRR = (1<<LED2) << 16;
	GPIOC->BSRR = (1<<LED3) << 16;
	GPIOA->BSRR = (1<<LED4) << 16;
*/
}
//void turn_led_on(struct led led) {
//
//	led->port->BSRR = (1<<led->pin);
//}
//
//void turn_led_off(struct led led) {
//
//	led->port->BSRR = ((1<<led->pin) << 16);
//
//}

// ----------------------------------------------------------------------------
