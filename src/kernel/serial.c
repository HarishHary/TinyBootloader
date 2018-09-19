#include <kernel/serial.h>

int write(const char* buf, size_t count) {
    for (size_t i = 0; i < count; i++)
        outb(COM1_Base, buf[i]);
    return count;
}
