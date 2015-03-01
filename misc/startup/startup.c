/**
 * \file startup.c
 * \brief Startup script for STM32
 *
 * \TODO extend interrupt vector table to support ALL interrupts
 */

#define STACK_START 0x20006000

/**
 * \brief Handler called in the start
 */
void irq_reset(void);
extern int main(void);

/**
 * \brief Interrupt vector table
 *
 * According to ARM spec. first word is an initial stack pointer
 * Second is a pointer to reset handler, which executes immediately
 * after stack pointer was set.
 *
 * Ideally, all interrupt handlers must be set in order to catch
 * any fault
 */
unsigned int *___interrupt_vectors[] __attribute__ ((section("vectors"))) = {
	(unsigned int *) STACK_START,		/** Initial SP value */
	(unsigned int *) irq_reset,		/** Reset */

	/** \TODO add handlers according
	 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0553a/BABIFJFG.html
	 */
};

void irq_reset(void) {
	/* Don't care about anything, just launch 'main' for a demo */
	main();
	/** \TODO perform chip reset */
	for (;;);
}
