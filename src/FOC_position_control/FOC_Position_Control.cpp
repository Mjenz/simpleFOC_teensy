#include <SimpleFOC.h>
#include <math.h>
#include <TeensyTimerTool.h>
#include "pos_controller.hpp"
#include "steps.hpp"

BLDCMotor motor = BLDCMotor(14,2.3); // pole pairs, phase resistance
BLDCDriver3PWM driver = BLDCDriver3PWM(4,5,6,3); // pwmA, pwmB, pwmC, Enable(optional)
MagneticSensorSPI encoder = MagneticSensorSPI(10, 14); // int cs, float _cpr
InlineCurrentSense current_sense  = InlineCurrentSense(.006, 50.0,_NC,22,17); // this is correct setup with c inverted

PositionController p_controller_{0.05, 0.0, 0.0, 0.0};

TeensyTimerTool::PeriodicTimer position_control_timer_(TeensyTimerTool::TCK);
TeensyTimerTool::PeriodicTimer print_timer_(TeensyTimerTool::TCK);

// init global vars
auto count = 0;
double target;
auto target_angle = 0.0;
auto next_angle = 0.0;
auto system_angle = 0.0;
auto system_vel = 0.0;
std::vector<float> trajectory;
char msg[100];

void trajectory_control_loop()
{

  // main FOC algorithm function
  motor.loopFOC();

  if (count > int(trajectory.size())-1)
  {
    count = 0;
  }

  // pump position controller
  target_angle = trajectory.at(count);
  system_angle =  encoder.getAngle();
  system_vel =  encoder.getVelocity();
  target = p_controller_.pump_controller(target_angle, system_angle, system_vel);

  // Motion control function
  motor.move(target);

  count++;
}

void print_loop()
{
  sprintf(msg,">setpoint: %f\n>system_pos: %f\n>system_vel: %f\n>error:%f\n>i_target:%f\n",target_angle, system_angle, system_vel,float(target_angle-system_angle), target);
  Serial.println(msg);

}

void setup() { 
  
  // initialize encoder sensor hardware
  encoder.init();

  // link the motor to the sensor
  motor.linkSensor(&encoder);

  // power supply voltage [V]
  driver.voltage_power_supply = 16.0;

  // init driver
  driver.init();

  // link driver
  motor.linkDriver(&driver);

  // link the driver to the current sense
  current_sense.linkDriver(&driver);
  
  // current sense init hardware
  current_sense.init();

  // link the current sense to the motor
  motor.linkCurrentSense(&current_sense);

  // set torque mode:
  motor.torque_controller = TorqueControlType::foc_current; 
  motor.controller = MotionControlType::torque;
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;

  // init pos controller

  // motor.PID_current_q.P = 0.05;
  // motor.PID_current_q.I=  10;
  // motor.PID_current_d.P= .05;
  // motor.PID_current_d.I = 10;
  // motor.LPF_current_q.Tf = 0.01; 
  // motor.LPF_current_d.Tf = 0.01; 

  // set current limit
  motor.current_limit = 3;

  // use monitoring with serial 
  Serial.begin(115200);

  // comment out if not needed
  motor.useMonitoring(Serial);
  SimpleFOCDebug::enable(&Serial);

  // initialize motor
  motor.init();

  // align sensor and start FOC
  if(!motor.initFOC()){
      while(1){}
  }

  // set initial motor target
  motor.target = 0.0;

  // get trajectory to follow
  std::copy(step_at_500ms.begin(), step_at_500ms.end(), std::back_inserter(trajectory));

  // wait 1 second
  delay(1000);

  // start position controller
  position_control_timer_.begin(
    [](){
      trajectory_control_loop();
    }, 100);

  // start print loop
  print_timer_.begin(
    [](){
      print_loop();
    }, 1000);

}

void loop() {}