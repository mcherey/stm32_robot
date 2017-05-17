#include <stdio.h>
#include <stdlib.h>

#include <test_motor.h>
#include <test_servo.h>
#include <test_sonar.h>
#include <timer.h>
#include <robot_car.h>
#include <test_monitor.h>
#include <iostream>


void BuildRobot()
{
  std::cout << "Setting up test modules:" << std::endl;
  std::cout << "Sonar...";
  TestSonar sonar;
  std::cout << " done" << std::endl;
  std::cout << "Servo...";
  TestServo servo;
  std::cout << " done" << std::endl;
  std::cout << "Motor...";
  TestMotor motor;
  std::cout << " done" << std::endl;
  std::cout << "Monitor...";
  TestMonitor monitor;
  std::cout << " done" << std::endl;

  std::cout << "Building robot...";
  Robot::RobotCar robot(monitor, sonar, servo, motor);
  std::cout << " done" << std::endl;

  std::cout << "Start execution" << std::endl;
  robot.Execute();
}

int main(int, char*[])
{

  std::cout << "Test program started." << std::endl;
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  BuildRobot();
  return 0;
}
