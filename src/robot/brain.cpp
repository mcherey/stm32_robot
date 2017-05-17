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

void Brain::PrintState()
{
  trace_printf("Action: %u, PrevAction: %u, (c: %u, l: %u, r: %u)\n",
               State.Action, State.PrevAction, State.DistanceCenter, State.DistanceLeft, State.DistanceRight);
}

int8_t Brain::ReleaseAction()
{
  State.PrevAction = State.Action;
  return State.Action;
}

void Brain::Analyze()
{
  State.Action = ACTION_ANALYZE;
}

void Brain::MakeDecision()
{
  //Initial state or after full stop
  if (State.Action == State.PrevAction && State.Action == ACTION_STOP)
  {
    State.Action = ACTION_EXAMINATION;
    return;
  }

  if (State.Action == ACTION_GO_AHEAD)
  {
    State.Action = ACTION_CHECK_OBSTACLE;
    return;
  }

  if (State.Action == ACTION_GO_BACK)
  {
    State.Action = ACTION_EXAMINATION;
    return;
  }

  if (State.Action == ACTION_TURN_LEFT)
  {
    State.Action = ACTION_EXAMINATION;
    return;
  }

  if (State.Action == ACTION_TURN_RIGHT)
  {
    State.Action = ACTION_EXAMINATION;
    return;
  }

  if (State.Action == ACTION_ANALYZE)
  {
    if (State.DistanceCenter > 100)
    {
      State.Action = ACTION_GO_AHEAD;
      return;
    }

    //if we can move forward
    if ((State.DistanceLeft > 100) && (State.DistanceLeft >= State.DistanceRight))
    {
      State.Action = ACTION_TURN_LEFT;
      return;
    }

    //if we can move forward
    if ((State.DistanceRight > 100) && (State.DistanceRight > State.DistanceLeft))
    {
      State.Action = ACTION_TURN_RIGHT;
      return;
    }

    State.Action = ACTION_STOP;
  }

}

}
