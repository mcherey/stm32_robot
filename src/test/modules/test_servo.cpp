#include "test_servo.h"

TestServo::TestServo()
{
}

TestServo::~TestServo ()
{
}

void TestServo::Left()
{
  Angle = -90;
}

void TestServo::Right()
{
  Angle = 90;
}

bool TestServo::IsCenter() const
{
  return (Angle == 0);
}

void TestServo::Center()
{
  Angle = 0;
}

void TestServo::SetAngle(int8_t angle)
{
  if (angle < -90)
  {
      Angle = -90;
  }
  else if (angle > 90)
  {
    Angle = 90;
  }
  else
  {
    Angle = angle;
  }
}

int8_t TestServo::GetAngle() const
{
  return Angle;
}
