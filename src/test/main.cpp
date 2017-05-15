#include <stdio.h>
#include <stdlib.h>

#include <test_motor.h>
#include <test_servo.h>
#include <test_sonar.h>
#include <timer.h>
#include <robot_car.h>
#include <test_monitor.h>


void BuildRobot()
{
  TestSonar sonar;
  TestServo servo;
  TestMotor motor;
  TestMonitor monitor;

  Robot::RobotCar robot(monitor, sonar, servo, motor);

  robot.Execute();
}

int main(int, char*[])
{
  BuildRobot();
  return 0;
}
