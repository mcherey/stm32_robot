#pragma once

#include <stdint.h>

namespace Robot
{
  class Servo
  {
  public:
    virtual ~Servo () {}

    virtual void Left() = 0;
    virtual void Right() = 0;
    virtual void Center() = 0;

    virtual bool IsCenter() const = 0;

    //-90..90
    virtual void SetAngle(int8_t angle) = 0;
    virtual int8_t GetAngle() const = 0;
  };
}
