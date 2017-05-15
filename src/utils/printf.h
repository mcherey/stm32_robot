#pragma once

#ifdef _WIN32
inline int trace_printf(const char* format, ...)
{
  return std::printf(format, ...);
}
#else
#include <diag/Trace.h>
#endif
