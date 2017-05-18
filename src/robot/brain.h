#pragma once

#include <stdint.h>

namespace Robot
{
  enum Action
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

  struct Memory
  {
    Action ToDo = ACTION_STOP;
    Action PrevAction = ACTION_STOP;
    bool CurrentActionExecuted = false;
    uint16_t DistanceLeft = 0;
    uint16_t DistanceRight = 0;
    uint16_t DistanceCenter = 0;
  };

  class Brain
  {
  public:
    Brain();
    virtual ~Brain();

    void ForgetDistance();
    void SetLeftDistance(uint16_t distance);
    void SetRightDistance(uint16_t distance);
    void SetCenterDistance(uint16_t distance);

    void PrintState(const char* msg = 0);
    void Analyze();
    void MakeDecision();

    Action ReleaseAction();

  private:
    void SetAction(Action state);
  private:
    Memory State;
  };

}
