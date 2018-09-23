.section .text
.macro idtentry sym:req num:req has_no_error_code=1
	.global \sym
	.type \sym, @function
\sym:
        .ifeq \has_no_error_code
          push $0
        .endif

        pusha
        push %ds
        push %es
        push %fs
        push %gs

        movl $\num, %eax
        pushl %eax

        call idt_print
        add $0x4, %esp

        pop %gs
        pop %fs
        pop %es
        pop %ds
        popa

        iret
        .size \sym, . - \sym
        .endm

        .extern idt_print
        idtentry isr0, 0
        idtentry isr1, 1
        idtentry isr2, 2
        idtentry isr3, 3
        idtentry isr4, 4
        idtentry isr5, 5
        idtentry isr6, 6
        idtentry isr7, 7
        idtentry isr8, 8, 0
        idtentry isr9, 9
        idtentry isr10, 10, 0
        idtentry isr11, 11, 0
        idtentry isr12, 12, 0
        idtentry isr13, 13, 0
        idtentry isr14, 14, 0
        idtentry isr15, 15
        idtentry isr16, 16
        idtentry isr17, 17, 0
        idtentry isr18, 18
        idtentry isr19, 19
        idtentry isr20, 20
        idtentry isr21, 21
        idtentry isr22, 22
        idtentry isr23, 23
        idtentry isr24, 24
        idtentry isr25, 25
        idtentry isr26, 26
        idtentry isr27, 27
        idtentry isr28, 28
        idtentry isr29, 29
        idtentry isr30, 30
        idtentry isr31, 31
