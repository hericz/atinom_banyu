MEMORY 
{
	flash	: ORIGIN = 0x00000000, LENGTH = 500K
	/* ram	: ORIGIN = 0x40000000, LENGTH = 32K */
	ram	: ORIGIN = 0x40000000, LENGTH = (32K - 32) 
	usbram   : ORIGIN = 0x7FD00000, LENGTH = 8K
	ethram   : ORIGIN = 0x7FE00000, LENGTH = 16K
}

__stack_end__ = 0x40000000 + 32K - 4;

SECTIONS 
{
	. = 0;
	startup : { *(.startup)} >flash

	prog : 
	{
		*(.text)
		*(.rodata)
		*(.rodata*)
		*(.glue_7)
		*(.glue_7t)
		*(.lokasi_font)
	} >flash

	__end_of_text__ = .;
	
	/* .ARM.exidx is sorted, so has to go in its own output section.  */
  __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } >flash
  __exidx_end = .;

	.data : 
	{
		__data_beg__ = .;
		__data_beg_src__ = __end_of_text__;
		*(.data)
		__data_end__ = .;
	} >ram AT>flash
  
	.bss : 
	{
		__bss_beg__ = .;
		*(.bss)
	} >ram

	/* Align here to ensure that the .bss section occupies space up to
	_end.  Align after .bss to ensure correct alignment even if the
	.bss section disappears because there are no input sections.  */
	
	. = ALIGN(32 / 8);
	_bss_end__ = . ; 
	__bss_end__ = . ; 
	__end__ = . ;

	/*.usbram (NOLOAD): */
	.usbram :
	{
	__usbram_beg__ = .;
	*(.dmaram)
		__usbram_end__ = .;
	} >usbram
	
	.ethram (NOLOAD):
	{
		__ethram_beg__ = .;
		*(.ethram)
		*(.eth_test_text)
		*(.eth_test)
		__ethram_end__ = .;
	} >ethram


}
	. = ALIGN(32 / 8);
	_end = .;
	
	PROVIDE (end = .);


