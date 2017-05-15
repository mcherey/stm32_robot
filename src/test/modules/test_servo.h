#pragma once

#include <servo.h>
#include <stdint.h>

class TestServo : public Robot::Servo
{
public:
  TestServo();
  virtual ~TestServo ();

  virtual void Left();
  virtual void Right();
  virtual void Center();

  virtual bool IsCenter() const;
  //-90..90
  virtual void SetAngle(int8_t angle);
  virtual int8_t GetAngle() const;
private:
  uint16_t Angle;
};

