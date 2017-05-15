#pragma once

#include <sonar.h>

class TestSonar : public Robot::Sonar
{
public:

  TestSonar();
  virtual ~TestSonar();

  virtual void Ping() override ;
  virtual uint16_t GetLastResult() override;

private:
};

