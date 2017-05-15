#include "robot_car.h"
#include <timer.h>
#include <printf.h>

namespace Robot
{

RobotCar::RobotCar(Monitor& monitor, Sonar& eyes, Servo& neck, Motor& legs)
  : M(monitor)
  , Eyes(eyes)
  , Neck(neck)
  , Legs(legs)
{
}

RobotCar::~RobotCar()
{
}

void RobotCar::Execute()
{
  Neck.Center();
  //wait to turn servo in necessary state
  Delay(1000);
  //robot.CheckAround();
  //robot.MakeDecision();

  while (true)
  {
    MakeDecision();
    trace_printf("prev: %u, action: %u\n", Data.PrevAction, Data.Action);
    Action();
    M.GreenLed(true);
    Delay(100);
    M.GreenLed(false);
  }
}

void RobotCar::CheckAround()
{
  M.RedLed(true);

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
  M.RedLed(false);

  trace_printf("CheckAround center: %u, left: %u, right: %u\n", Data.DistanceCenter, Data.DistanceLeft, Data.DistanceRight);
  Data.Action = ACTION_ANALYZE;
}

void RobotCar::MakeDecision()
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

bool RobotCar::IsStateChanged()
{
  return Data.Action != Data.PrevAction;
}

void RobotCar::Action()
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

void RobotCar::CheckObstacle()
{
  M.RedLed(true);
  if (!Neck.IsCenter())
  {
    Neck.Center();
  }

  //Update distance
  Eyes.Ping();
  Delay(60);
  Data.DistanceCenter = Eyes.GetLastResult();
  M.RedLed(false);
  Data.Action = ACTION_ANALYZE;
}

void RobotCar::Idle()
{
  Delay(100);
}

void RobotCar::Stop()
{
  trace_printf("STOP\n");
  Legs.SoftStop();
}

void RobotCar::GoAhead()
{
  trace_printf("FORWARD\n");
  Legs.Forward();
}

void RobotCar::GoBack()
{
  trace_printf("BACK\n");
  Legs.Back();
  //Delay(1000);
  Delay(500);
}

void RobotCar::GoLeft()
{
  trace_printf("LEFT\n");
  Legs.Left();
  //Delay(1000);
  Delay(500);
}

void RobotCar::GoRight()
{
  trace_printf("RIGHT\n");
  Legs.Right();
  Delay(500);
}

}
