SECTIONS
{
	. = 0x7C00;
	.text : AT(0x7C00)
	{
		*(.start)
		*(.text*)
	}
	.data :
	{
		_data = .;
		*(.bss*);
		*(.data*);
		*(.rodata*);
		_data_end = .;
	}
	. = 0x7DFE;
	.signature : AT(0x7DFE)
	{
		SHORT(0xaa55);
	}
	/DISCARD/ :
	{
		*(.note*);
		*(.iplt*);
		*(.igot*);
		*(.rel*);
		*(.comment);
	}
}
