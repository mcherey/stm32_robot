#pragma once

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>

class Led
{
public:
  Led(GPIO_TypeDef* gpio, uint16_t pin);
  virtual ~Led();

  void Enable(bool enable);

private:
  GPIO_TypeDef* GPIOX;
  uint16_t Pin;
};
