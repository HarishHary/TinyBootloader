#pragma once

#include <kernel/io.h>

#define COM1 0x3F8

int write(const char* buf, size_t count);
