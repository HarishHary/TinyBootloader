#include <kernel/serial.h>

void init_serial() {
   outb(COM1 + 1, 0x00);    // Disable all interrupts
   outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(COM1 + 1, 0x00);    //                  (hi byte)
   outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

u32 is_transmit_empty() {
   return inb(COM1 + 5) & 0x20;
}

int write(const char* buf, size_t count) {
    static u32 initialized = 0;

    if (!initialized)
    {
      init_serial();
      initialized = 1;
    }

    while (is_transmit_empty() == 0);

    for (size_t i = 0; i < count; i++)
        outb(COM1, buf[i]);
    return count;
}
