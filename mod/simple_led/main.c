/*
 * This example shows LEDs blinking with some simple algorithm.
 * LEDs are attached to the GPIO pins on the STM32 discovery board.
 * Prerequisites are next:
 *  - Enabled peripheral clock for corresponding GPIO pins.
 *
 *    Corresponding pins for LEDs could be found in
 *    STM32 UM1472 'User manual, Discovery kit for STM32F407/417 lines',
 *    section 4.4 'LEDs'
 *
 *    GPIO configuration described in
 *    STM32 RM0090 'Reference manual for STM32F405xx/07xx, STM32F415xx/17xx,
 *    STM32F42xxx and STM32F43xxx advanced ARM-based 32-bit MCUs',
 *    section 8 'General-purpose I/Os (GPIO)'
 *
 *    How clocks are coupled in whole system described in RM0090,
 *    Figure 21. 'Clock tree'
 *
 *    Clock registers, responsible for specific GPIO port may be found in
 *    RM0090, section 7.3.25 'RCC register map'
 *
 *  - Initialized GPIO as output, without push\pull.
 *
 *    Registers, responsible for GPIO configuration and other useful data may
 *    be found in RM0090, section 8 'General-purpose I/Os (GPIO)'
 */

#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

int main(void)
{
	volatile unsigned int i, j;
	/* Our pins, basically */
	uint16_t pins = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12;

	/* Enabling peripheral clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef gpio_init = { 0 };

	GPIO_StructInit(&gpio_init);
	gpio_init.GPIO_Pin = pins;
	gpio_init.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init.GPIO_Speed = GPIO_High_Speed;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/* GPIO is enabled here */
	GPIO_Init(GPIOD, &gpio_init);

	/* Cycle between four on-board LEDs of STM32F4 DISCOVERY */
	for(j = 0;; j++)
	{
		uint16_t pin = (GPIO_Pin_12 << (j % 4)) & pins;
		for (i = 0; i < 500000; i++);
		GPIO_SetBits(GPIOD, pin);
		for (i = 0; i < 500000; i++);
		GPIO_ResetBits(GPIOD, pin);
	}

	return 0;
}
