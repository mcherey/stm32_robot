#pragma once

namespace Robot
{
  class Led
  {
  public:
    virtual ~Led() {}
    virtual void Enable(bool enable) = 0;
  };
}
