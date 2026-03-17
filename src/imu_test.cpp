#include "lsm6dsv.hpp"
#include <TeensyTimerTool.h>

TeensyTimerTool::PeriodicTimer imu_timer_(TeensyTimerTool::TCK);
TeensyTimerTool::PeriodicTimer step_detect_timer_(TeensyTimerTool::TCK);
TeensyTimerTool::PeriodicTimer print_timer_(TeensyTimerTool::TCK);

LSM6DSV_IMU imu;

static accelerations imu_data;

void imu_loop()
{
  imu_data = imu.get_acc();
}

// void step_detect_loop()
// {
  
// }

void print_loop()
{
  Serial.print(">ax:");
  Serial.println(imu_data.x, 3);

  Serial.print(">ay:");
  Serial.println(imu_data.y, 3);

  Serial.print(">az:");
  Serial.println(imu_data.z, 3);
}
void setup() 
{
  // init serial
  Serial.begin(115200);

  // init imu
  imu.init();

  // start imu loop
  imu_timer_.begin(
    [](){
      imu_loop();
    }, 1000);

  // start print loop
  print_timer_.begin(
    [](){
      print_loop();
    }, 10000);
}

void loop() 
{
}