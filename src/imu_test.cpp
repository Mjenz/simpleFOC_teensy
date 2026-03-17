#include "lsm6dsv.hpp"
#include <TeensyTimerTool.h>

TeensyTimerTool::PeriodicTimer imu_timer_(TeensyTimerTool::TCK);

LSM6DSV_IMU imu;

void imu_loop()
{
  auto imu_data = imu.get_acc();

  Serial.println(">> ")
}
void setup() 
{
  // init serial
  Serial.begin(115200);

  // init imu
  imu.init();
}

void loop() 
{
}