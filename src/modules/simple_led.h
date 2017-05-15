#pragma once

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <led.h>

class SimpleLed : public Robot::Led
{
public:
  SimpleLed(GPIO_TypeDef* gpio, uint16_t pin);
  virtual ~SimpleLed();

  virtual void Enable(bool enable) override;

private:
  GPIO_TypeDef* GPIOX;
  uint16_t Pin;
};
