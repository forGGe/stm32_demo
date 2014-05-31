/* Just a memory regions */
MEMORY
{
    /* TODO: Clarify this */
    rom (rx)     : ORIGIN = 0x00000000, LENGTH = 0x04000 /* 16K */
    ram (rwx)    : ORIGIN = 0x20000000, LENGTH = 0x2c000 /* 172 K */
    flash (rwx)  : ORIGIN = 0x08000000, LENGTH = 1M
}

/* This should be point to actual start point, for ARM it is a
 *  reset handler. Defined in main startup file
 */
ENTRY(irq_reset)

SECTIONS
{
    /* .text section goes to rom memory region, starts from 0x0,
     * aligned by 4, contains vectors section inside 
     * (defined inside main source file) and the text section itself.
     */

    .text :
    {
       . = ALIGN(4);
       KEEP(*(vectors))
       KEEP(*(.text))
    } > rom

    /* .bss goes to ram */
    .bss  : { *(.bss*) } > ram
}