#pragma once

#ifdef STM32F10X_MD
#include <stm32f10x.h>
#else
#include <stdint.h>
#endif

void InitTimer();

void Delay(uint32_t time);
