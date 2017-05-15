#pragma once

#ifdef STM32F10X_MD
#include <diag/Trace.h>
#else
#include <stdio.h>
#define trace_printf(...) printf(__VA_ARGS__)
#endif
