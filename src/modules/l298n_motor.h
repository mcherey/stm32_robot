#include <stm32f10x.h>
#include <motor.h>

#pragma once

class L298NMotor : public Robot::Motor
{
public:
  L298NMotor(GPIO_TypeDef* gpio1, uint16_t enA, uint16_t inA1, uint16_t inA2,
             uint16_t enB, uint16_t inB1, uint16_t inB2);
  virtual ~L298NMotor();

  virtual void Forward() override;
  virtual void Back() override;
  virtual void Right() override;
  virtual void Left() override;
  virtual void SoftStop() override;
  virtual void HardStop() override;
private:
  void LeftMotorsForward();
  void LeftMotorsBack();
  void RightMotorsForward();
  void RightMotorsBack();

private:
  GPIO_TypeDef* GPIOX;
  uint16_t EnA;
  uint16_t InA1;
  uint16_t InA2;
  uint16_t EnB;
  uint16_t InB1;
  uint16_t InB2;
};

