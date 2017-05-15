#include <stdio.h>
#include <stdlib.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_exti.h>

#include <diag/Trace.h>
#include <l298n_motor.h>
#include <servo_sg90.h>
#include <sonar_hcsr04.h>
#include <spi.h>
#include <step_motor.h>
#include <timer.h>
#include <led.h>

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
  while(1);
}
#endif

struct State
{
  int8_t Action = 0;
  int8_t PrevAction = 0;
  uint16_t DistanceLeft = 0;
  uint16_t DistanceRight = 0;
  uint16_t DistanceCenter = 0;
};

struct Robot
{
  enum
  {
    ACTION_STOP = 0,
    ACTION_EXAMINATION,
    ACTION_CHECK_OBSTACLE,
    ACTION_ANALYZE,
    ACTION_IDLE,
    ACTION_GO_AHEAD,
    ACTION_TURN_LEFT,
    ACTION_TURN_RIGHT,
    ACTION_GO_BACK
  };

  Led RedLed;
  Led GreenLed;

  SonarHCSR04& Eyes;
  ServoSG90& Neck;
  L298NMotor& Legs;

  Robot(SonarHCSR04& eyes, ServoSG90& neck, L298NMotor& legs)
    : RedLed(GPIOB, GPIO_Pin_8)
    , GreenLed(GPIOB, GPIO_Pin_7)
    , Eyes(eyes)
    , Neck(neck)
    , Legs(legs)
  {
  }

  void CheckAround()
  {
    RedLed.Enable(true);

    Data.DistanceCenter = 0;
    Data.DistanceLeft = 0;
    Data.DistanceRight = 0;

    Neck.Left();
    //wait to turn servo in necessary state
    if (Neck.GetAngle() <= 0)
    {
      Delay(500);
    }
    else
    {
      //need more time to turn from right
      Delay(1000);
    }
    Eyes.Ping();
    Delay(60);
    Data.DistanceLeft = Eyes.GetLastResult();

    Neck.Center();
    //wait to turn servo in necessary state
    Delay(500);
    Eyes.Ping();
    Delay(60);
    Data.DistanceCenter = Eyes.GetLastResult();

    Neck.Right();
    //wait to turn servo in necessary state
    Delay(500);
    Eyes.Ping();
    Delay(60);
    Data.DistanceRight = Eyes.GetLastResult();

    Neck.Center();
    //wait to turn servo in necessary state
    Delay(500);
    RedLed.Enable(false);

    trace_printf("CheckAround center: %u, left: %u, right: %u\n", Data.DistanceCenter, Data.DistanceLeft, Data.DistanceRight);
    Data.Action = ACTION_ANALYZE;
  }

  void MakeDecision()
  {
    //Initial state or after full stop
    if (Data.Action == Data.PrevAction && Data.Action == ACTION_STOP)
    {
      Data.Action = ACTION_EXAMINATION;
      return;
    }

    if (Data.Action == ACTION_GO_AHEAD)
    {
      Data.Action = ACTION_CHECK_OBSTACLE;
      return;
    }

    if (Data.Action == ACTION_GO_BACK)
    {
      Data.Action = ACTION_EXAMINATION;
      return;
    }

    if (Data.Action == ACTION_TURN_LEFT)
    {
      Data.Action = ACTION_EXAMINATION;
      return;
    }

    if (Data.Action == ACTION_TURN_RIGHT)
    {
      Data.Action = ACTION_EXAMINATION;
      return;
    }

    if (Data.Action == ACTION_ANALYZE)
    {
      if (Data.DistanceCenter > 100)
      {
        Data.Action = ACTION_GO_AHEAD;
        return;
      }

      //if we can move forward
      if ((Data.DistanceLeft > 100) && (Data.DistanceLeft >= Data.DistanceRight))
      {
        Data.Action = ACTION_TURN_LEFT;
        return;
      }

      //if we can move forward
      if ((Data.DistanceRight > 100) && (Data.DistanceRight > Data.DistanceLeft))
      {
        Data.Action = ACTION_TURN_RIGHT;
        return;
      }

      Data.Action = ACTION_STOP;
    }

  }

  bool IsStateChanged()
  {
    return Data.Action != Data.PrevAction;
  }

  void Action()
  {
    Data.PrevAction = Data.Action;
    //Data.Action = ACTION_ANALYZE;
    switch(Data.Action)
    {
      case ACTION_STOP: Stop(); break;
      case ACTION_EXAMINATION: Stop(); CheckAround(); break;
      case ACTION_CHECK_OBSTACLE: CheckObstacle(); break;
      case ACTION_IDLE: Stop(); Idle(); break;
      case ACTION_GO_AHEAD: GoAhead(); break;
      case ACTION_GO_BACK: Stop(); GoBack(); break;
      case ACTION_TURN_LEFT: Stop(); GoLeft(); break;
      case ACTION_TURN_RIGHT: Stop(); GoRight(); break;
    }
  }

  void CheckObstacle()
  {
    RedLed.Enable(true);
    if (!Neck.IsCenter())
    {
      Neck.Center();
    }

    //Update distance
    Eyes.Ping();
    Delay(60);
    Data.DistanceCenter = Eyes.GetLastResult();
    RedLed.Enable(false);
    Data.Action = ACTION_ANALYZE;
  }

  void Idle()
  {
    Delay(100);
  }

  void Stop()
  {
    trace_printf("STOP\n");
    Legs.SoftStop();
  }

  void GoAhead()
  {
    trace_printf("FORWARD\n");
    Legs.Forward();
  }

  void GoBack()
  {
    trace_printf("BACK\n");
    Legs.Back();
    //Delay(1000);
    Delay(500);
  }

  void GoLeft()
  {
    trace_printf("LEFT\n");
    Legs.Left();
    //Delay(1000);
    Delay(500);
  }

  void GoRight()
  {
    trace_printf("RIGHT\n");
    Legs.Right();
    Delay(500);
  }
public:
  State Data;
};

void robot()
{
  SonarHCSR04 sonar(GPIOB, GPIO_Pin_15);
  ServoSG90 servo(TIM2, 2, GPIOA, GPIO_Pin_1);
  L298NMotor motor(GPIOA, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_7,
                   GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10);

  Robot robot(sonar, servo, motor);

  robot.Neck.Center();
  //wait to turn servo in necessary state
  Delay(1000);
  //robot.CheckAround();
  //robot.MakeDecision();

  while (true)
  {
    robot.MakeDecision();
    trace_printf("prev: %u, action: %u\n", robot.Data.PrevAction, robot.Data.Action);
    robot.Action();
    robot.GreenLed.Enable(true);
    Delay(100);
    robot.GreenLed.Enable(false);
  }
  //*/
}

void initClock()
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
  initClock();
  robot();
  return 0;
}

#pragma GCC diagnostic pop
