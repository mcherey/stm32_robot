#include <brain.h>
#include <printf.h>

namespace Robot
{
Brain::Brain()
{
}

Brain::~Brain()
{
}

void Brain::ForgetDistance()
{
  State.DistanceCenter = 0;
  State.DistanceLeft = 0;
  State.DistanceRight = 0;
}

void Brain::SetLeftDistance(uint16_t distance)
{
  State.DistanceLeft = distance;
}

void Brain::SetRightDistance(uint16_t distance)
{
  State.DistanceRight = distance;
}

void Brain::SetCenterDistance(uint16_t distance)
{
  State.DistanceCenter = distance;
}

void Brain::PrintState(const char* msg)
{
  trace_printf("[%s] ToDo: %u, PrevAction: %u, (c: %u, l: %u, r: %u)\n",
               msg ? msg : "Info",
               State.ToDo, State.PrevAction, State.DistanceCenter,
               State.DistanceLeft, State.DistanceRight);
}

Action Brain::ReleaseAction()
{
  if (State.CurrentActionExecuted)
  {
    trace_printf("[ERROR] Trying to execute already executed action. ToDo: %u, PrevAction: %u",
                 State.ToDo, State.PrevAction);
  }

  State.PrevAction = State.ToDo;
  return State.ToDo;
}

void Brain::SetAction(Action action)
{
  if (!State.CurrentActionExecuted)
  {
    trace_printf("[ERROR] Trying to set new action but current wasn't executed yet. New action: %u, ToDo: %u, PrevAction: %u",
                 action, State.ToDo, State.PrevAction);
  }
  State.PrevAction = State.ToDo;
  State.ToDo = action;
}

void Brain::Analyze()
{
  SetAction(ACTION_ANALYZE);
}

void Brain::MakeDecision()
{
  //Initial state or after full stop
  if (State.ToDo == State.PrevAction && State.ToDo == ACTION_STOP)
  {
    State.ToDo = ACTION_EXAMINATION;
    return;
  }

  if (State.ToDo == ACTION_GO_AHEAD)
  {
    State.ToDo = ACTION_CHECK_OBSTACLE;
    return;
  }

  if (State.ToDo == ACTION_GO_BACK)
  {
    State.ToDo = ACTION_EXAMINATION;
    return;
  }

  if (State.ToDo == ACTION_TURN_LEFT)
  {
    State.ToDo = ACTION_EXAMINATION;
    return;
  }

  if (State.ToDo == ACTION_TURN_RIGHT)
  {
    State.ToDo = ACTION_EXAMINATION;
    return;
  }

  if (State.ToDo == ACTION_ANALYZE)
  {
    if (State.DistanceCenter > 100)
    {
      State.ToDo = ACTION_GO_AHEAD;
      return;
    }

    //if we can move forward
    if ((State.DistanceLeft > 100) && (State.DistanceLeft >= State.DistanceRight))
    {
      State.ToDo = ACTION_TURN_LEFT;
      return;
    }

    //if we can move forward
    if ((State.DistanceRight > 100) && (State.DistanceRight > State.DistanceLeft))
    {
      State.ToDo = ACTION_TURN_RIGHT;
      return;
    }

    State.ToDo = ACTION_STOP;
  }

}

}
