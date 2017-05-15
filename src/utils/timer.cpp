#include "timer.h"

#ifdef STM32F10X_MD
static __IO uint32_t TimingDelay;
#else
#include <unistd.h>
#endif

void InitTimer()
{
#ifdef STM32F10X_MD
  if (SysTick_Config(SystemCoreClock / 1000))
  {
   while(1);
  }
#endif
}

void Delay(uint32_t time)
{
#ifdef STM32F10X_MD
  TimingDelay = time;
  while(TimingDelay != 0);
#else
  usleep(time*1000);
#endif
}

#ifdef STM32F10X_MD
extern "C"
void SysTick_Handler(void)
{
  if (TimingDelay != 0x00)
    TimingDelay--;
}
#endif
