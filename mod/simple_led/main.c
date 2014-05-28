#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define STACK_START 0x20001000
void handler_reset(void);

/*
 * Interrupt vector table
 * According to ARM spec. first word is an initial stack pointer
 * Second is a pointer to reset handler, which executes immediately
 * after stack pointer was set.
 *
 * Ideally, all interrupt handlers must be set in order to catch
 * any fault
 */
unsigned int *__vector_handlers[] __attribute__ ((section(".vectors"))) = {
    (unsigned int *) STACK_START,		/* Initial stack pointer */
    (unsigned int *) handler_reset,		/* Reset handler pointer */
};

void handler_reset(void) {
	/* Don't care about anything, just launch 'main' for a demo*/
	main();
}

int main(void)
{
	int i, j;
	uint16_t pins = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef gpio_init = { 0 };

	GPIO_StructInit(&gpio_init);
	gpio_init.GPIO_Pin = pins;
	gpio_init.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init.GPIO_Speed = GPIO_High_Speed;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD, &gpio_init);

	/* Cycle between four on-board LEDs for STM32F4 DISCOVERY */
	for(j = 0;; j++)
	{
		uint16_t pin = (GPIO_Pin_12 << (j % 4)) & pins;
		for (i = 0; i < 500000; i++);
		GPIO_SetBits(GPIOD, pin);
		for (i = 0; i < 500000; i++);
		GPIO_ResetBits(GPIOD, pin);
	}
}