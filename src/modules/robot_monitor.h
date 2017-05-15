#pragma once

#include <monitor.h>
#include <simple_led.h>

namespace Robot
{
  class RobotMonitor : public Robot::Monitor
  {
  public:
    RobotMonitor();
    virtual ~RobotMonitor();

    virtual void RedLed(bool enable) override;
    virtual void GreenLed(bool enable) override;

  private:
    SimpleLed Red;
    SimpleLed Green;
  };
}
