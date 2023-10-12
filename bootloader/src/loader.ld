ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);

MEMORY
{
  CCMRAM		(xrw)	: ORIGIN = 0x10000000,	LENGTH = 64K
  RAM    		(xrw)	: ORIGIN = 0x20000000,	LENGTH = 192K
  FLASH    		(rx)	: ORIGIN = 0x8000000,	LENGTH = 32K
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
		_sromdev = .;
		_eromdev = .;
		_sidata = .;
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

	.API_SHARED 0x81F5000 :
	{
		KEEP(*(.API_SHARED))
	} > MY_MEMORY

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
	} >RAM

}