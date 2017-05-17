#include "test_servo.h"
#include <iostream>

TestServo::TestServo()
  : Angle(0)
{
}

TestServo::~TestServo ()
{
}

void TestServo::Left()
{
  std::cout << "[TestServo]::Left" << std::endl;
  Angle = -90;
}

void TestServo::Right()
{
  std::cout << "[TestServo]::Right" << std::endl;
  Angle = 90;
}

bool TestServo::IsCenter() const
{
  std::cout << "[TestServo]::IsCenter " << (Angle == 0) << std::endl;
  return (Angle == 0);
}

void TestServo::Center()
{
  std::cout << "[TestServo]::Center" << std::endl;
  Angle = 0;
}

void TestServo::SetAngle(int8_t angle)
{
  std::cout << "[TestServo]::SetAngle" << angle << std::endl;
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
  std::cout << "[TestServo]::GetAngle" << int(Angle) << std::endl;
  return Angle;
}
