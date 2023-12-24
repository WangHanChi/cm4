ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);

MEMORY
{
  CCMRAM		(xrw)	: ORIGIN = 0x10000000,	LENGTH = 64K
  RAM    		(xrw)	: ORIGIN = 0x20000000,	LENGTH = 192K
  FLASH    		(rx)	: ORIGIN = 0x800C000,	LENGTH = 1952K
  MY_MEMORY		(rx)	: ORIGIN = 0x81F4000,	LENGTH = 48K
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

	.mysection : 
	{
		. = ALIGN(4);
		__mysection_start__ = .;
		*(.mysection*)
		__mysection_end__ = .;
	} >MY_MEMORY

	/* check if MY_MEMORY usage exceeds MY_MEMORY size */
	ASSERT(LENGTH(MY_MEMORY) >= (__mysection_end__ - __mysection_start__), "MY_MEMORY memory overflowed !")

	/* this will place my named section at given address */
	.myBufBlockRAM 0x20000100 (NOLOAD): 
	{
		KEEP(*(.myBufSectionRAM))	/* keep my variable even if not referenced */
	} >RAM

	/* this will place my named section at given address */
	.myBufBlockFLASH 0x8001000 : 
	{
		KEEP(*(.myBufSectionFLASH))	/* keep my variable even if not referenced */
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
		*(.RamFunc)        /* .RamFunc sections */
    	*(.RamFunc*)       /* .RamFunc* sections */

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