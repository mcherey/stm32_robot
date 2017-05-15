#include <stdio.h>
#include <stdlib.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_exti.h>

#include <l298n_motor.h>
#include <servo_sg90.h>
#include <simple_led.h>
#include <sonar_hcsr04.h>
#include <spi.h>
#include <step_motor.h>
#include <timer.h>
#include <robot_car.h>
#include <robot_monitor.h>

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
  while(1);
}
#endif

void BuildRobot()
{
  SonarHCSR04 sonar(GPIOB, GPIO_Pin_15);
  ServoSG90 servo(TIM2, 2, GPIOA, GPIO_Pin_1);
  L298NMotor motor(GPIOA, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_7,
                   GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10);

  Robot::RobotMonitor monitor;
  Robot::RobotCar robot(monitor, sonar, servo, motor);

  robot.Execute();
}

void InitClock()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

int main(int, char*[])
{
  InitTimer();
  InitClock();
  BuildRobot();
  return 0;
}

#pragma GCC diagnostic pop
