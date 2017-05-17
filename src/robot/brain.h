#pragma once

#include <stdint.h>

namespace Robot
{
  struct Memory
  {
    int8_t Action = 0;
    int8_t PrevAction = 0;
    uint16_t DistanceLeft = 0;
    uint16_t DistanceRight = 0;
    uint16_t DistanceCenter = 0;
  };

  class Brain
  {
  public:
    enum
    {
      ACTION_STOP = 0,
      ACTION_EXAMINATION,
      ACTION_CHECK_OBSTACLE,
      ACTION_ANALYZE,
      ACTION_IDLE,
      ACTION_GO_AHEAD,
      ACTION_TURN_LEFT,
      ACTION_TURN_RIGHT,
      ACTION_GO_BACK
    };
  public:
    Brain();
    virtual ~Brain();

    void ForgetDistance();
    void SetLeftDistance(uint16_t distance);
    void SetRightDistance(uint16_t distance);
    void SetCenterDistance(uint16_t distance);

    void PrintState();
    void Analyze();
    void MakeDecision();
    int8_t ReleaseAction();

  private:
    Memory State;
  };

}
