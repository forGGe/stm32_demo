/*
 * Example is showing how to implement simple wait function that waits for some
 * amount of cycles.
 *
 * DWT (data watchpoint register) is used here. DWT is a capability defined by
 * Cortex specification. STM32F4 doesn't contain sufficient documentation of
 * this register. Example below was found somewhere in the Internet.
 *
 * NOTE: Calculations of cycles aren't accurate of course, since some cycles
 * are took place during side processes, i.e. function calls.
 *
 * References:
 * - STM32 RM0090 Reference manual for STM32F405xx/07xx, STM32F415xx/17xx,
 *   STM32F42xxx and STM32F43xxx advanced ARM®-based 32-bit MCUs,
 *   section 38.13
 * - Stackoverflow answer:
 *   http://stackoverflow.com/questions/11530593/cycle-counter-on-arm-cortex-m4-or-m3
 * - Kviba's article:
 *   https://kbiva.wordpress.com/2013/03/25/microsecond-delay-function-for-stm32/
 */

#include <stdbool.h>
#include "stm32f4xx.h"

/* Amount of cycles per each wait */
#define CYCLES_TO_WAIT  10000

/* Amount of total iteration */
#define ITERATIONS      1000

/* Inits DWT */
static void DWT_Init(void)
{
	if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

	/* Reset cycle counter */
	DWT->CYCCNT = 0;

	if (!(DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk))
		DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/* Disables DWT */
static void DWT_Shutdown(void)
{
	if (DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk)
		DWT->CTRL &= ~(DWT_CTRL_CYCCNTENA_Msk);

	if (CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)
		CoreDebug->DEMCR &= ~(CoreDebug_DEMCR_TRCENA_Msk);

}

/* Gets current value of cycle counter */
static uint32_t DWT_Get(void)
{
	return DWT->CYCCNT;
}

/* Returs true if given value bigger than amount of currently passed cycles */
static bool DWT_Compare(uint32_t cycles)
{
	/* For this example it's OK to use int instead of unsigned here */
	return DWT_Get() < cycles;
}

/* Basically, waits for given amount of cycles */
static void DWT_cycles(uint32_t cycles)
{
	DWT_Init();
	while (DWT_Compare(cycles));
	DWT_Shutdown();
}

int main(void)
{
	volatile unsigned i = 0;

	for (i; i < ITERATIONS; ++i) {
		DWT_cycles(CYCLES_TO_WAIT);
	}
}

