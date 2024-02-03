ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);

MEMORY
{
  CCMRAM		(xrw)	: ORIGIN = 0x10000000,	LENGTH = 64K
  RAM    		(xrw)	: ORIGIN = 0x20000000,	LENGTH = 192K
  PASSWORD   	(rx)	: ORIGIN = 0x800B000,   LENGTH = 4K
  FLASH    		(rx)	: ORIGIN = 0x800C000,	LENGTH = 1952K
}

SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vector))
		*(.text)
		*(.text.*)
		*(.rodata)
		*(.rodata.*)
		_sromdev = .;
		_eromdev = .;
	} >FLASH

	.PASSWORD 0x800B000:
	{
		KEEP(*(.PASSWORD))
	} >PASSWORD

	.ARM.excep :			/* Data table for library exception handling */
	{
		*(.ARM.exidx*)
		*(.ARM.extab*)
	} >FLASH

	. = ALIGN(4);
	_etext = .;				/* End of .text section */
	_sidata = .;			/* Start of the initialization values of the .data section */

	.data : AT(_sidata)
	{
		_sdata = .;
		*(.data)
		*(.data*)
		_edata = .;
	} >RAM

	.bss :
	{
		_sbss = .;
		*(.bss)
		_ebss = .;
		end = .;
	} >RAM

}