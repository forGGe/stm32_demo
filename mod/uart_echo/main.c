/*
 * This example shows how to work with U(S)ART on STM32F4
 *
 * USART is an universal synchronous/asynchronous receiver/transmitter.
 * Here and below name 'UART' may be used, since example covers only
 * asynchronous mode.
 *
 * 'minicom' host application could connect to the UART tty.
 *
 * For convenience the default minicom's configuration will be used, as below:
 *  - baud rate: 115200
 *  - word size: 8
 *  - stop bits: 1
 *  - parity: none
 *  - hardware flow control: none
 *
 * *** Don't forget to use '-D' switch when running minicom!
 * *** This will use default settings.
 *
 * General procedure is following:
 *  - enable clock for UART peripheral bus
 *  - enable clock for GPIO peripheral bus, on which UART pins reside
 *  - configure pins alternate functions
 *  - enable corresponding GPIO port
 *  - configure UART parameters, such as baud rate or word size
 *  - enable UART
 *  - write\read data from it. This covered in detail below
 *
 * Before each transmission starts (from MCU to world, or TX), application
 * has to know that UART is ready for sending data. For this purpose,
 * corresponding bit should be checked. If this bit high, then data could be
 * written to the data register.
 *
 * Same states for RX UART.
 *
 * Note that RX and TX are coupled inside one register. Writing to it results
 * in using TX stream, reading results in using RX.
 *
 * Pins:
 *  PA0 - TX, AF8
 *  PA1 - RX, AF8
 *
 * References:
 *   - What is UART, wiki:
 *     http://en.wikipedia.org/wiki/Universal_asynchronous_receiver/transmitter
 *
 *   - Difference between UART and USART, briefly:
 *     http://electronics.stackexchange.com/questions/33886/uart-usart-whats-the-difference
 *
 *   - Proper UART could be chosen from
 *     STM32F405xx, STM32F407xx, 'Datasheet - production data'
 *
 *     Section 2.2.23, 'Universal synchronous/asynchronous receiver transmitters (USART)'
 *     provides some brief info about capabilities of USART
 *
 *     Table 5, 'USART feature comparison' covers main set of feature for
 *     each USART present on MCU
 *
 *   - GPIO that sutable for UART purposes may be found in
 *     table 9, 'Alternate function mapping' in STM32F405xx, STM32F407xx,
 *     'Datasheet - production data'
 *
 *   - Corresponding peripheral bus, that supplies UART and GPIOs is taken from
 *     section 2.2, 'Device overview' and figure 5, 'STM32F40x block diagram'
 *
 *   - In depth, USART on STM32F4 is covered in RM0090, 'Reference manual',
 *     section 30 'Universal synchronous asynchronous receiver transmitter (USART)'
 *     How to properly send and receive data covered there as well.
 */


#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_gpio.h>

#include <stddef.h>

#define UART_BAUD_RATE  115200   /* Our baud rate, default for minicom */
#define RECV_SIZE       128      /* Size of buffer to recieve data */

/* Our implementation for memcpy(). This is used internally by compiler */
void *memcpy(void *dest, const void *src, size_t n)
{
	uint8_t *d = dest;
	const uint8_t *s = src;
	while (n--) { *d++ = *s++; }
	return dest;
}

/* Same as usual strncmp() */
int strncmp(const char *s1, const char *s2, size_t n)
{
	int d = 0;
	while (n-- && (d = *s1++ - *s2++) == 0);
	return d;
}

/* Sends character to the UART. Blocks until UART will be ready
 * to receive data
 */
static void send_char(char ch)
{
	/* This bit should be set before any
	 * outgoing transaction will take place
	 */
	while (!(UART4->SR & USART_SR_TC)) {}
	USART_SendData(UART4, (uint16_t) ch);
}

/* Receive character from the UART. Blocks until UART will be ready
 * to provide any data
 */
static char recv_char(void)
{
	/* This bit should be set before any incoming
	 * data will be read
	 */
	while (!(UART4->SR & USART_SR_RXNE)) {}
	return (char) USART_ReceiveData(UART4);
}

/* Sends a null-terminated string to the UART */
static void send_string(const char *buf)
{
	char ch;
	while ((ch = *buf++) != '\0')
		send_char(ch);
}

/* Receives data from UART, echoing back any character typed.
 * Waits until carriage return will be received and then null-terminates
 * resulting string. Returns amount of bytes written including null-character.
 */
static size_t recv_string(char *buf, size_t size)
{
	if (!size)
		return 0;

	char ch = 0;
	size_t i = 0;

	while (ch != '\r' && i < size)
	{
		ch = recv_char();
		/* Sends back received character */
		send_char(ch);
		buf[i++] = ch;
	}

	buf[i - 1] = 0;
	return i;
}


int main(void)
{
	/* Greeting string, printed on a very start */
	const char greetings[] =
		"\r\nHello World!\r\n"
		"Type 'Quit' to exit programm\r\n"
		"Press Enter to see echo:\r\n";

	/* Buffer to store incoming data */
	char recv[RECV_SIZE];

	/* Enables clock on UART and corresponding GPIO port */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Initializes GPIO pins */

	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio_init.GPIO_Mode = GPIO_Mode_AF;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_Speed = GPIO_Fast_Speed;
	gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/* PA0 and PA1 pins should be configured as UART AF (AF8) */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);

	GPIO_Init(GPIOA, &gpio_init);

	/* Initializes UART itself */

	USART_InitTypeDef usart_init;
	usart_init.USART_BaudRate = UART_BAUD_RATE;
	usart_init.USART_WordLength = USART_WordLength_8b;
	usart_init.USART_StopBits = USART_StopBits_1;
	usart_init.USART_Parity = USART_Parity_No;
	usart_init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_Init(UART4, &usart_init);

	/* Enables UART */
	USART_Cmd(UART4, ENABLE);

	send_string(greetings);

	/* Main echo loop.
	 * Idea: read string, show it to the screen, exit by keyword
	 */
	for (;;)
	{
		send_string("\r\n$ ");
		size_t recieved = recv_string(recv, RECV_SIZE);
		send_string("\r\nYou've typed: \r\n");
		send_string(recv);
		send_string("\r\n");

		if (0 == strncmp(recv, "Quit", recieved))
		{
			send_string("Exiting...\r\n");
			break;
		}
	}

	return 0;
}
