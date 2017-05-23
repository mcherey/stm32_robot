#include <brain.h>
#include <printf.h>

namespace Robot
{
  namespace Constants
  {
    const uint16_t MinDistance = 100;
  }

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
    trace_printf("[ERROR] Trying to execute already executed action. ToDo: %u, PrevAction: %u\n",
                 State.ToDo, State.PrevAction);
  }

  State.CurrentActionExecuted = true;
  return State.ToDo;
}

void Brain::DoNext(Action action)
{
  if (!State.CurrentActionExecuted)
  {
    trace_printf("[ERROR] Trying to set new action but current wasn't executed yet. New action: %u, ToDo: %u, PrevAction: %u\n",
                 action, State.ToDo, State.PrevAction);
  }
  State.PrevAction = State.ToDo;
  State.ToDo = action;
  State.CurrentActionExecuted = false;
}

Action Brain::ChooseDirection()
{
  if ((State.DistanceCenter + State.DistanceLeft + State.DistanceRight) < Constants::MinDistance)
  {
    return ACTION_ANALYZE;
  }

  if (State.DistanceCenter >= 3 * Constants::MinDistance)
  {
    return ACTION_GO_AHEAD;
  }

  if ((State.DistanceLeft >= 100) && (State.DistanceLeft >= State.DistanceRight))
  {
    return ACTION_TURN_LEFT;
  }

  //if we can move forward
  if ((State.DistanceRight >= 100) && (State.DistanceRight > State.DistanceLeft))
  {
    return ACTION_TURN_RIGHT;
  }

  return ACTION_GO_BACK;
}

Action Brain::Analyze()
{
  if (State.PrevAction == ACTION_STOP)
  {
    return ACTION_EXAMINATION;
  }

  if (State.PrevAction == ACTION_EXAMINATION)
  {
    return ChooseDirection();
  }

  if (State.PrevAction == ACTION_CHECK_OBSTACLE)
  {
    return ChooseDirection();
  }

  if (State.PrevAction == ACTION_GO_AHEAD)
  {
    return ACTION_CHECK_OBSTACLE;
  }

  if (State.PrevAction == ACTION_TURN_LEFT)
  {
    return ACTION_STOP;
  }

  if (State.PrevAction == ACTION_TURN_RIGHT)
  {
    return ACTION_STOP;
  }

  return ACTION_STOP;
}

Action Brain::MakeDecision()
{
  //Initial state or after full stop
  if (State.ToDo == State.PrevAction && State.ToDo == ACTION_STOP)
  {
    return ACTION_ANALYZE;
  }

  if (State.ToDo != ACTION_ANALYZE)
  {
    trace_printf("[ERROR] No need to make decision. ToDo: %u, PrevAction: %u\n",
                 State.ToDo, State.PrevAction);
    return State.ToDo;
  }

  auto wasToDo = State.ToDo;
  auto wasPrevAction = State.PrevAction;
  auto result = Analyze();
  trace_printf("[ANALYZE] ToDo: %u, PrevAction: %u ==> %u\n",
               wasToDo, wasPrevAction, result);
  return result;
}
}
