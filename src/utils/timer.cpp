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

  // DWT_Init
  if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
  {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  }
#endif
}

void DelayUs(uint32_t time)
{
#ifdef STM32F10X_MD
  int32_t tp = DWT->CYCCNT + time * (SystemCoreClock/1000000);
  while (((int32_t)DWT->CYCCNT - tp) < 0);
#else
  usleep(time);
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
