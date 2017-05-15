#pragma once

namespace Robot
{
  class Sonar
  {
  public:
    virtual ~Sonar() {}

    virtual void Ping() = 0;
    virtual uint16_t GetLastResult() = 0;
  };
}
