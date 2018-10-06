ENTRY(main)

SECTIONS
{
	. = 0x7C00;
	.stage1_text : AT(0x7C00)
	{
		*(.stage1)
		stage1.o(.text*)
		drive.o(.text*)
	}
	.stage1_data :
	{
		_data = .;
		stage1.o(.bss*)
		stage1.o(.data*)
		stage1.o(.rodata*)
		drive.o(.bss*)
		drive.o(.data*)
		drive.o(.rodata*)
		_data_end = .;
	}

	. = 0x7DFE;
	.signature : AT(0x7DFE)
	{
		SHORT(0xAA55);
	}

	.stage2_text :
  {
    *(.stage2)
    *(.text*)
  }
  .stage2_data :
  {
	  _data = .;
    *(.bss*);
    *(.data*);
    *(.rodata*);
		_data_end = .;
  }

	.padding_end :
	{
		BYTE(0x0);
	  . = ALIGN(512) - 1;
		BYTE(0x0);
	}

	/DISCARD/ :
	{
		*(.note*);
		*(.iplt*);
		*(.igot*);
		*(.rel*);
		*(.comment*);
	}
}
