#include <stm32f10x.h>

#pragma once

class ServoSG90
{
public:
  ServoSG90(TIM_TypeDef* tim, uint8_t timChannel, GPIO_TypeDef* gpio, uint16_t signalPort);
  virtual ~ServoSG90 ();

  void Left();
  void Right();
  void Center();
  //-90..90
  void SetAngle(int8_t angle);
  int8_t GetAngle() const;
private:
  TIM_TypeDef* TIMX;
  int16_t Angle;
};

