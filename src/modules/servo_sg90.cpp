#include "../modules/servo_sg90.h"

#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>

ServoSG90::ServoSG90(TIM_TypeDef* tim, uint8_t timChannel, GPIO_TypeDef* gpio, uint16_t signalPort)
  : TIMX(tim)
  , Angle(0)
{
  GPIO_InitTypeDef port;
  GPIO_StructInit(&port);
  port.GPIO_Mode = GPIO_Mode_AF_PP;
  port.GPIO_Pin = signalPort;
  port.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &port);

  TIM_TimeBaseInitTypeDef timer;
  TIM_OCInitTypeDef timerPWM;

  TIM_TimeBaseStructInit(&timer);
  timer.TIM_Prescaler = SystemCoreClock/(50*20000) - 1;  //SystemCoreClock/(PWM frequency*PWM resolution) – 1
  timer.TIM_Period = 20000;
  timer.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIMX, &timer);

  TIM_OCStructInit(&timerPWM);
  timerPWM.TIM_Pulse = 1000;
  timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
  timerPWM.TIM_OutputState = TIM_OutputState_Enable;
  switch (timChannel)
  {
    case 1: TIM_OC1Init(TIMX, &timerPWM); break;
    case 2: TIM_OC2Init(TIMX, &timerPWM); break;
    case 3: TIM_OC3Init(TIMX, &timerPWM); break;
    case 4: TIM_OC4Init(TIMX, &timerPWM); break;

  }

  TIM_Cmd(tim, ENABLE);
}

ServoSG90::~ServoSG90 ()
{
}

void ServoSG90::Left()
{
  Angle = -90;
  TIM_SetCompare2(TIMX, 2500);
}

void ServoSG90::Right()
{
  Angle = 90;
  TIM_SetCompare2(TIMX, 600);
}

bool ServoSG90::IsCenter() const
{
  return (Angle == 0);
}

void ServoSG90::Center()
{
  Angle = 0;
  TIM_SetCompare2(TIMX, 1500);
}

void ServoSG90::SetAngle(int8_t angle)
{
  if (angle < -90)
  {
      Angle = -90;
  }
  else if (angle > 90)
  {
    Angle = 90;
  }
  else
  {
    Angle = angle;
  }
  uint16_t a = uint16_t(Angle + 90);
  TIM_SetCompare2(TIMX, 600 + (a * 10.6));
}

int8_t ServoSG90::GetAngle() const
{
  return Angle;
}
