#include <stdio.h>
#include <stdlib.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_exti.h>

#include <diag/Trace.h>
#include "modules/l298n_motor.h"
#include "modules/servo_sg90.h"
#include "modules/sonar_hcsr04.h"
#include "modules/spi.h"
#include "modules/step_motor.h"
#include <timer.h>



#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
  while(1);
}
#endif

char L_Data[5];         // array data for left motor
uint8_t L_index = 0;    // index of array L
char R_Data[5];         // array data for right motor
uint8_t R_index = 0;    // index of array R
char H_Data[1];         // array data for additional channel
uint8_t H_index = 0;    // index of array H
char F_Data[8];         // array data for  Flash
uint8_t F_index = 0;    // index of array F

char command;           // command
char incomingByte;
int valueL, valueR;     // PWM value M1, M2


void motor()
{

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  L298NMotor motor(GPIOA, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_7,
                   GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10);

  while (1)
  {
    motor.Forward();
    Delay(1000);
    motor.Back();
    Delay(1000);
    motor.SoftStop();
    Delay(1000);
    motor.Left();
    Delay(1000);
    motor.Right();
    Delay(1000);
    motor.HardStop();
    Delay(1000);
  }
}

/*
void spi_test()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  Spi spi(SPI2);
  spi.Test();
}
*/


void robot()
{

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);


  SonarHCSR04 sonar(GPIOB, GPIO_Pin_15);
  ServoSG90 servo(TIM2, 2, GPIOA, GPIO_Pin_1);
  L298NMotor motor(GPIOA, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_7,
                   GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10);
  while (true)
  {
    motor.Forward();
    servo.SetAngle(-90);
    Delay(1000);
    sonar.Ping();
    motor.Right();
    servo.SetAngle(-45);
    Delay(1000);
    trace_printf("sonar left: %u\n", sonar.GetLastResult());

    motor.SoftStop();
    servo.SetAngle(0);
    Delay(1000);

    servo.SetAngle(45);
    Delay(1000);
    servo.SetAngle(90);
    Delay(1000);
    sonar.Ping();
    Delay(1000);
    trace_printf("sonar right: %u\n", sonar.GetLastResult());
  }


}

int main(int argc, char* argv[])
{
  InitTimer();
  //spi_test();
  //motor();

  robot();
/*
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  SonarHCSR04 sonar(GPIOB, GPIO_Pin_15);

  while (1)
  {
    sonar.Ping();
    Delay(1000);
    trace_printf("sonar: %u\n", sonar.GetLastResult());

  }

  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  //ServoSG90 servo(TIM2, 2, GPIOA, GPIO_Pin_1);
/*
  while (true)
  {
    servo.SetAngle(-90);
    Delay(1000);
    servo.SetAngle(-45);
    Delay(1000);
    servo.SetAngle(0);
    Delay(1000);
    servo.SetAngle(45);
    Delay(1000);
    servo.SetAngle(90);
    Delay(1000);
  }

 */

  /*

  // Enable PORTB Clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  StepMotor motor(GPIOB, GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9);
  while(1)
  {
      motor.StepMotorNextStep();

    Delay(2);
  }
*/

  /*
  // Show the program parameters (passed via semihosting).
  // Output is via the semihosting output channel.
  trace_dump_args(argc, argv);

  // Send a greeting to the trace device (skipped on Release).
  trace_puts("Hello ARM World!");

  // Send a message to the standard output.
  puts("Standard output message.");

  // Send a message to the standard error.
  fprintf(stderr, "Standard error message.\n");

  // At this stage the system clock should have already been configured
  // at high speed.
  trace_printf("System clock: %u Hz\n", SystemCoreClock);

  Timer timer;
  timer.start();

  BlinkLed blinkLed;

  // Perform all necessary initialisations for the LED.
  blinkLed.powerUp();

  uint32_t seconds = 0;

#define LOOP_COUNT (5)
  int loops = LOOP_COUNT;
  if (argc > 1)
    {
      // If defined, get the number of loops from the command line,
      // configurable via semihosting.
      loops = atoi (argv[1]);
    }

  // Short loop.
  for (int i = 0; i < loops; i++)
    {
      blinkLed.turnOn();
      timer.sleep(i == 0 ? Timer::FREQUENCY_HZ : BLINK_ON_TICKS);

      blinkLed.turnOff();
      timer.sleep(BLINK_OFF_TICKS);

      ++seconds;

      // Count seconds on the trace device.
      trace_printf("Second %u\n", seconds);
    }
  return 0;
  */
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
