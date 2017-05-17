#pragma once

#ifdef STM32F10X_MD
#include <diag/Trace.h>
#else
#include <stdarg.h>
#include <stdio.h>
inline void trace_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
#endif
