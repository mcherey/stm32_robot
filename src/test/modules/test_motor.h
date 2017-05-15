#include <motor.h>

#pragma once

class TestMotor : public Robot::Motor
{
public:
  TestMotor();
  virtual ~TestMotor();

  virtual void Forward() override;
  virtual void Back() override;
  virtual void Right() override;
  virtual void Left() override;
  virtual void SoftStop() override;
  virtual void HardStop() override;

private:
};

