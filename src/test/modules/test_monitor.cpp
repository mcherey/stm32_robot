#include "test_monitor.h"


TestMonitor::TestMonitor()
  : Red(false)
  , Green(false)
{
}

TestMonitor::~TestMonitor()
{
}

void TestMonitor::RedLed(bool enable)
{
  Red = enable;
}

void TestMonitor::GreenLed(bool enable)
{
  Green = enable;
}

