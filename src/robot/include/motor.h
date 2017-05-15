#pragma once

namespace Robot
{
  class Motor
  {
  public:
    virtual ~Motor() {}

    virtual void Forward();
    virtual void Back();
    virtual void Right();
    virtual void Left();
    virtual void SoftStop();
    virtual void HardStop();
  };
}
