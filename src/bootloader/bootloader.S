.code16
.section    .data
    base:   .long 0, 0
    length: .long 0, 0
    type:   .long 0
    acpi:   .long 0
    temp:   .long 0
    sectorpertrack:   .long 0
    numberofheads:    .long 0
    drive:  .byte 0

.section    .text
.global     _start
_start:
mov %dx, drive
call _do_e820
call _read_param_drive
call _read_section_drive
call _init_gdt
hlt

########### CHECK A20 LINE ########### (FIXME)
_check_A20:
    mov $0xffff, %bx
    mov %bx, %ds
    mov $0x7e0e, %bx
    mov (%bx), %dx
    cmp $0xaa55, %dx
    jne _check_A20_SUCCESS

    shr $8, %bx

    mov (%bx), %dx
    cmp $0xaa55, %dx
    jne _check_A20_SUCCESS
    mov $0, %bx
    ret
_check_A20_SUCCESS:
    mov $1, %ax
    ret

########### DIFFERENT TECHNIQUES TO ENABLE A20 LINE ###########
_enable_A20_BIOS:
    mov $0x2401, %ax
    int $0x15
    ret

_enable_A20_KBC:
    cli
    call _a20wait
    mov $0xAD, %al
    outb %al, $0x64
    call _a20wait
    mov $0xD0, %al
    outb %al, $0x64
    call _a20wait2
    inb $0x60, %al
    push %eax
    call _a20wait
    mov $0xD1, %al
    outb %al, $0x64
    call _a20wait
    pop %eax
    or $2, %al
    outb %al, $0x60
    call _a20wait
    mov $0xAE, %al
    outb %al, $0x64
    call _a20wait
    sti
    ret
_a20wait:
    in $0x64, %al
    test $2, %al
    jnz _a20wait
    ret
_a20wait2:
    inb $0x64, %al
    test $1, %al
    jz _a20wait2
    ret

_enable_A20_FAST_GATE:
    inb $0x92, %al
    or $2, %al
    outb %al, $0x92
    ret

########### ENABLE A20 ###########
_enable_A20:
    call _check_A20
    cmp  $0, %ax
    jne  _enable_A20_EXIT
    call _enable_A20_BIOS
    call _check_A20
    cmp  $0, %ax
    jne  _enable_A20_EXIT
    call _enable_A20_KBC
    call _check_A20
    cmp  $0, %ax
    jne  _enable_A20_EXIT
    call _enable_A20_FAST_GATE
    call _check_A20
    xchg %bx, %bx
    cmp $0, %ax
    jne  _enable_A20_ERROR
_enable_A20_EXIT:
    ret
_enable_A20_ERROR:
    hlt

########### FIND AVAILABLE RAM ZONE ###########
_do_e820:
    xorl %ebx, %ebx
_do_e820_loop:
    movl $0x0534D4150, %edx
    movl $0xe820, %eax
    movw $base, %di
    movl $24, %ecx
    int $0x15
    jc _do_e820_error
    cmp $0x0534D4150, %eax
    jne _do_e820_error
    cmpl $0x1, (type)
    jne _do_e820_testloop
    cmpl $0xffff, (base)
    jg _do_e820_end
_do_e820_testloop:
    test %ebx, %ebx
    je _do_e820_error
    jmp _do_e820_loop
_do_e820_end:
    ret
_do_e820_error:
    hlt

########### LOAD MY CODE FROM DISK ###########
_read_param_drive:
    mov $0, %eax
    mov %ax, %es
    mov $0, %di
    mov $0x08, %ah
    mov (drive), %dl
    int $0x13
    jc _read_param_drive_error
    ret
_read_param_drive_error:
    hlt

_read_section_drive:
    mov %dh, numberofheads
    add $1, numberofheads
    and $0x3f, %cl
    mov %cl, sectorpertrack

    #temp
    xor %ax, %ax
    mov $1, %al
    mov (sectorpertrack), %dl
    div %dl
    mov %al, temp

    #sector
    inc %ah
    mov %ah, %cl

    #head
    xor %ax, %ax
    mov (temp), %al
    mov (numberofheads), %dl
    div %dl
    mov %ah, %dh

    #cylinder
    mov %al, %ch

    #calc selector
    mov $0xffff, %ax
    mov %ax, %es
    cmpl $0x100000, (base)
    jge _selector_calc_end

_selector_calc:
    movl (base), %eax
    shr $0x4, %eax
    and $0xf000, %eax
    movl %eax, %es
_selector_calc_end:

    #calc offset
    mov (base), %eax
    and $0xffff, %eax
    cmpl $0x100000, (base)
    jl _offset_calc_end
    add $0x10, %eax
_offset_calc_end:
    mov %ax, temp

    #calc sector count
    mov $KERNEL_SIZE, %ax
    mov $0x200, %bx
    div %bx
    cmp $0x0, %dx
    je _read_drive
    inc %al

_read_drive:
    mov $0x02, %ah
    mov (drive), %dl
    mov (temp), %bx
    int $0x13
    jc _read_section_drive_error
    ret
_read_section_drive_error:
    hlt

########### INIT GDT ###########
gdt_begin:
gdt_null_segment:       #null descriptor, mandatory for some reason
    .long 0x00
    .long 0x00
gdt_code_segment:    #describes segment where code is stored
    .word 0xffff
    .word 0x00
    .byte 0x00
    .byte 0b10011011
    .byte 0b11001111
    .byte 0x00
gdt_data_segment:    #describes segment where data is stored
    .word 0xffff
    .word 0x00
    .byte 0x00
    .byte 0b10010011
    .byte 0b11001111
    .byte 0x00
gdt_end:
gdt_descriptor:
    .word gdt_end - gdt_begin - 1
    .long gdt_begin

    .equ CODE_SEG, gdt_code_segment - gdt_begin
    .equ DATA_SEG, gdt_data_segment - gdt_begin

_init_gdt:
    lgdt gdt_descriptor
    mov %cr0, %eax
    orl $0x1, %eax
    cli
    mov %eax, %cr0
    mov $DATA_SEG, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    ljmp $CODE_SEG, $_protected_mode

.code32
_protected_mode:
    jmp *base
