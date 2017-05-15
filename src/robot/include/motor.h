#pragma once

namespace Robot
{
  class Motor
  {
  public:
    virtual ~Motor() {}

    virtual void Forward() = 0;
    virtual void Back() = 0;
    virtual void Right() = 0;
    virtual void Left() = 0;
    virtual void SoftStop() = 0;
    virtual void HardStop() = 0;
  };
}
