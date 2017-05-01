#include "timer.h"

static __IO uint32_t TimingDelay;

void InitTimer()
{
  if (SysTick_Config(SystemCoreClock / 1000))
  {
   while(1);
  }
}

void Delay(uint32_t time)
{
  TimingDelay = time;
  while(TimingDelay != 0);
}

extern "C"
void SysTick_Handler(void)
{
  if (TimingDelay != 0x00)
    TimingDelay--;
}
