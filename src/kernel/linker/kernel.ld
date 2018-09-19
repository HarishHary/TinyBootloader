ENTRY(main);

SECTIONS
{
	. = 0x100000;
	.text : AT(0x100000)
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
	.padding_start : {
		BYTE(0x0);
	}
	. = ALIGN(512) - 1;
	.padding_end : {
		BYTE(0x0);
	}
}
