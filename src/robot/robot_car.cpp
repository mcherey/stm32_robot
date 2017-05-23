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
  trace_printf("Starting robot main thread.\n");
  Neck.Center();
  //wait to turn servo in necessary state
  Delay(1000);
  //CheckAround();
  //B.MakeDecision();

  B.PrintState("Initial state");

  while (true)
  {
    //B.PrintState("BEGIN");
    Action();
    /*Eyes.Ping();
    Delay(60);
    uint16_t value = Eyes.GetLastResult();
    trace_printf("value %u\n", value);
*/
    //M.GreenLed(false);
    //M.GreenLed(true);
    //Delay(100);
    //M.GreenLed(false);
    //Delay(1000);
  }
}

void RobotCar::Action()
{
  auto action = B.ReleaseAction();

  auto nextAction = ACTION_ANALYZE;
  switch(action)
  {
    case ACTION_STOP: Stop(); break;
    case ACTION_EXAMINATION: CheckAround(); break;
    case ACTION_CHECK_OBSTACLE: CheckObstacle(); break;
    case ACTION_IDLE: Stop(); Idle(); break;
    case ACTION_GO_AHEAD: GoAhead(); break;
    case ACTION_GO_BACK: GoBack(); break;
    case ACTION_TURN_LEFT: GoLeft(); break;
    case ACTION_TURN_RIGHT: GoRight(); break;
    case ACTION_ANALYZE: nextAction = B.MakeDecision(); break;
  }

  B.DoNext(nextAction);
}

void RobotCar::CheckObstacle()
{
  trace_printf("CheckObstacle\n");
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
}

void RobotCar::CheckAround()
{
  trace_printf("CheckAround\n");
  M.RedLed(true);

  B.ForgetDistance();

  int8_t angle = Neck.GetAngle();
  Neck.Left();
  //wait to turn servo in necessary state
  if (angle > -50 && angle <= 10)
  {
    Delay(500);
  }
  else if (angle > 10)
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
}

void RobotCar::Idle()
{
  trace_printf("IDLE\n");
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
  Delay(200);
}

void RobotCar::GoLeft()
{
  trace_printf("LEFT\n");
  Legs.Left();
  Delay(200);
}

void RobotCar::GoRight()
{
  trace_printf("RIGHT\n");
  Legs.Right();
  Delay(200);
}

}
