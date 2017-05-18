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
  trace_printf("Statring robot main thread.\n");
  Neck.Center();
  //wait to turn servo in necessary state
  Delay(1000);
  //CheckAround();
  //B.MakeDecision();

  B.PrintState("Initial state");

  //GoAhead();
 // Delay(5000);
  //GoBack();
  //Delay(5000);

  while (true)
  {
    B.MakeDecision();
    B.PrintState();
    Action();
    M.GreenLed(true);
    Delay(100);
    M.GreenLed(false);
  }
}

void RobotCar::CheckAround()
{
  M.RedLed(true);

  B.ForgetDistance();

  int8_t angle = Neck.GetAngle();
  Neck.Left();
  //wait to turn servo in necessary state
  if (angle <= 0)
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
  B.SetLeftDistance(Eyes.GetLastResult());

  Neck.Center();
  //wait to turn servo in necessary state
  Delay(500);
  Eyes.Ping();
  Delay(60);
  B.SetCenterDistance(Eyes.GetLastResult());

  Neck.Right();
  //wait to turn servo in necessary state
  Delay(500);
  Eyes.Ping();
  Delay(60);
  B.SetRightDistance(Eyes.GetLastResult());

  Neck.Center();
  //wait to turn servo in necessary state
  Delay(500);
  M.RedLed(false);

  B.PrintState();
  B.Analyze();
}

void RobotCar::Action()
{
  auto action = B.ReleaseAction();

  switch(action)
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
  B.SetCenterDistance(Eyes.GetLastResult());
  M.RedLed(false);
  B.Analyze();
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
