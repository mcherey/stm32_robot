#pragma once

#include <stm32f10x.h>
#include <servo.h>

class ServoSG90 : public Robot::Servo
{
public:
  ServoSG90(TIM_TypeDef* tim, uint8_t timChannel, GPIO_TypeDef* gpio, uint16_t signalPort);
  virtual ~ServoSG90 ();

  virtual void Left();
  virtual void Right();
  virtual void Center();

  virtual bool IsCenter() const;
  //-90..90
  virtual void SetAngle(int8_t angle);
  virtual int8_t GetAngle() const;
private:
  TIM_TypeDef* TIMX;
  int16_t Angle;
};

