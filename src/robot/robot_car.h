#pragma once

#include "brain.h"

#include <motor.h>
#include <monitor.h>
#include <servo.h>
#include <sonar.h>

namespace Robot
{
  class RobotCar
  {
  public:
    RobotCar(Monitor& monitor, Sonar& eyes, Servo& neck, Motor& legs);
    virtual ~RobotCar();

    void Execute();

    void CheckAround();

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

    Brain B;
  };
}
