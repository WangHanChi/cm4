ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);

MEMORY
{
  CCMRAM		(xrw)	: ORIGIN = 0x10000000,	LENGTH = 64K
  RAM    		(xrw)	: ORIGIN = 0x20000000,	LENGTH = 192K
  FLASH    		(rx)	: ORIGIN = 0x8000000,	LENGTH = 32K
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