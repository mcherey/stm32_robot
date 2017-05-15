#pragma once

namespace Robot
{
  class Monitor
  {
  public:
    virtual ~Monitor() {}

    virtual void RedLed(bool enable) = 0;
    virtual void GreenLed(bool enable) = 0;
  };
}
