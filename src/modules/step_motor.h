#include <stm32f10x.h>

#pragma once

class StepMotor
{
public:
  StepMotor(GPIO_TypeDef* gpio, uint16_t in1, uint16_t in2, uint16_t in3,
            uint16_t in4);
  virtual ~StepMotor ();

  void StepMotorNextStep(void);
  void StepMotorPrevStep(void);

private:
  const uint16_t* StepSequence;
  uint8_t MotorStep;
};

