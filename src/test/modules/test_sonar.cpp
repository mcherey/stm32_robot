#include "test_sonar.h"
#include <timer.h>

TestSonar::TestSonar()
{
}

TestSonar::~TestSonar()
{
}

void TestSonar::Ping()
{
}

uint16_t TestSonar::GetLastResult()
{
  unsigned long Sonar = 120;
  return (uint16_t)Sonar;
}
