#pragma once

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>

class SonarHCSR04
{
public:
  //GPIOB GPIO_Pin_0 - input
  //Using TIM3
  SonarHCSR04(GPIO_TypeDef* triggerGPIOX, uint16_t triggerPin);
  virtual ~SonarHCSR04();

  void Ping();
  uint32_t GetLastResult();

private:
  TIM_TypeDef* TIMX;
  GPIO_TypeDef* TriggerGPIOX;
  uint16_t TriggerPin;
};

