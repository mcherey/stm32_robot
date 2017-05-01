#include "step_motor.h"
#include <stm32f10x_gpio.h>

StepMotor::StepMotor (GPIO_TypeDef* gpio, uint16_t in1, uint16_t in2, uint16_t in3,
                      uint16_t in4)
: StepSequence(0)
, MotorStep(0)
{
  static const uint16_t Sequence[] = {in1, in1|in2, in2, in2|in3, in3, in3|in4, in4, in4|in1};
  StepSequence = Sequence;

  GPIO_InitTypeDef  GPIO_InitStructure;

  // PORT Clock should be enabled!
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* Configure the pins */
  GPIO_InitStructure.GPIO_Pin = in1 | in2 | in3 | in4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(gpio, &GPIO_InitStructure);
}

StepMotor::~StepMotor ()
{
}

void StepMotor::StepMotorNextStep(void)
{
  MotorStep++;
  if (MotorStep > 7)
    MotorStep = 0;

  GPIO_Write(GPIOB, StepSequence[MotorStep]);
}

void StepMotor::StepMotorPrevStep(void)
{
  if (MotorStep > 0)
    MotorStep--;
  else
    MotorStep = 7;

  GPIO_Write(GPIOB, StepSequence[MotorStep]);
}
