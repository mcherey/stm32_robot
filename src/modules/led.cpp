#include <led.h>

Led::Led(GPIO_TypeDef* gpio, uint16_t pin)
  : GPIOX(gpio)
  , Pin(pin)
{
  GPIO_InitTypeDef pinConfiguration;
  GPIO_StructInit(&pinConfiguration);

  pinConfiguration.GPIO_Mode = GPIO_Mode_Out_PP;
  pinConfiguration.GPIO_Pin = Pin;
  GPIO_Init(GPIOX, &pinConfiguration);
  GPIO_ResetBits(GPIOX, Pin);
}

Led::~Led()
{
}

void Led::Enable(bool enable)
{
  if (enable)
  {
    GPIO_SetBits(GPIOX, Pin);
  }
  else
  {
    GPIO_ResetBits(GPIOX, Pin);
  }
}
