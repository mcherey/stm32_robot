#pragma once

#include <motor.h>
#include <monitor.h>
#include <servo.h>
#include <simple_led.h>
#include <sonar.h>

namespace Robot
{
  struct State
  {
    int8_t Action = 0;
    int8_t PrevAction = 0;
    uint16_t DistanceLeft = 0;
    uint16_t DistanceRight = 0;
    uint16_t DistanceCenter = 0;
  };

  class RobotCar
  {
  public:
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
  public:
    RobotCar(Monitor& monitor, Sonar& eyes, Servo& neck, Motor& legs);
    virtual ~RobotCar();

    void Execute();

    void CheckAround();
    void MakeDecision();
    bool IsStateChanged();
    void Action();
    void CheckObstacle();
    void Idle();
    void Stop();
    void GoAhead();
    void GoBack();
    void GoLeft();

    void GoRight();
  private:
    Monitor& M;

    Sonar& Eyes;
    Servo& Neck;
    Motor& Legs;

    State Data;
  };
}
