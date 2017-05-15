#pragma once

#include <monitor.h>

class TestMonitor : public Robot::Monitor
{
public:
  TestMonitor();
  virtual ~TestMonitor();

  virtual void RedLed(bool enable) override;
  virtual void GreenLed(bool enable) override;

private:
  bool Red;
  bool Green;
};
