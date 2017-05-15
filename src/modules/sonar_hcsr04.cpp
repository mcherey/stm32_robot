#include "sonar_hcsr04.h"

#include <stm32f10x_tim.h>
#include <stm32f10x_exti.h>
#include <timer.h>

volatile uint16_t SonarValue;

extern "C"
void EXTI0_IRQHandler(void)
{
  /* Make sure that interrupt flag is set */
  if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) != 0) {
      // Rising
      TIM_SetCounter(TIM3, 0);
    }
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {
      // Falling
      SonarValue = TIM_GetCounter(TIM3);
    }

    /* Clear interrupt flag */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

SonarHCSR04::SonarHCSR04(GPIO_TypeDef* triggerGPIOX, uint16_t triggerPin)
  : TIMX(TIM3)
  , TriggerGPIOX(triggerGPIOX)
  , TriggerPin(triggerPin)
{
  //Trigger Pin
  GPIO_InitTypeDef gpio_cfg;
  GPIO_StructInit(&gpio_cfg);

  gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;
  gpio_cfg.GPIO_Pin = TriggerPin;
  GPIO_Init(TriggerGPIOX, &gpio_cfg);

  /* Timer TIMX settings */
  TIM_TimeBaseInitTypeDef timer_base;
  TIM_TimeBaseStructInit(&timer_base);
  timer_base.TIM_CounterMode = TIM_CounterMode_Up;
  timer_base.TIM_Prescaler = 72 - 1;
  TIM_TimeBaseInit(TIMX, &timer_base);
  TIM_Cmd(TIMX, ENABLE);


  //EXTI
  /* Set variables used */
  GPIO_InitTypeDef GPIO_InitStruct;
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  /* Set pin as input */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Add IRQ vector to NVIC */
  /* PB0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */
  NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
  /* Set priority */
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
  /* Set sub priority */
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
  /* Enable interrupt */
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  /* Add to NVIC */
  NVIC_Init(&NVIC_InitStruct);

  /* Tell system that you will use PB0 for EXTI_Line0 */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);

  /* PB0 is connected to EXTI_Line0 */
  EXTI_InitStruct.EXTI_Line = EXTI_Line0;
  /* Enable interrupt */
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  /* Interrupt mode */
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  /* Triggers on rising and falling edge */
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  /* Add to EXTI */
  EXTI_Init(&EXTI_InitStruct);
}

SonarHCSR04::~SonarHCSR04()
{
}

void SonarHCSR04::Ping()
{
  int i;

  SonarValue = 0;
  GPIO_SetBits(TriggerGPIOX, TriggerPin);
  //Delay 0x72000
  for(i=0;i<0x7200;i++);
  //Delay(1);
  GPIO_ResetBits(TriggerGPIOX, TriggerPin);
}

uint16_t SonarHCSR04::GetLastResult()
{
  unsigned long Sonar;
  //Sonar = (unsigned long)SonarValue / (58.2);
  Sonar = (354/2) * (unsigned long)SonarValue / (72000 / 72);
  //if (Sonar > 4000) Sonar = 4000;
  //if (Sonar < 20) Sonar = 20;
  return (uint16_t)Sonar;
}
