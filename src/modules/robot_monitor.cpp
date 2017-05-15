#include "robot_monitor.h"

namespace Robot
{
RobotMonitor::RobotMonitor()
  : Red(GPIOB, GPIO_Pin_8)
  , Green(GPIOB, GPIO_Pin_7)
{
}

RobotMonitor::~RobotMonitor()
{
}

void RobotMonitor::RedLed(bool enable)
{
  Red.Enable(enable);
}

void RobotMonitor::GreenLed(bool enable)
{
  Green.Enable(enable);
}

}
