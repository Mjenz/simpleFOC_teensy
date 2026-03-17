#include <SimpleFOC.h>
#include <math.h>
#include <TeensyTimerTool.h>
#include "pos_controller.hpp"
#include "step_detection.hpp"
#include "lsm6dsv.hpp"

// trajectory headers
#include "steps.hpp"
#include "swing_traj_11.hpp"

// simpleFOC class initializations
BLDCMotor motor = BLDCMotor(14,2.3); // pole pairs, phase resistance
BLDCDriver3PWM driver = BLDCDriver3PWM(4,5,6,3); // pwmA, pwmB, pwmC, Enable(optional)
MagneticSensorSPI encoder = MagneticSensorSPI(10, 14); // int cs, float _cpr
InlineCurrentSense current_sense  = InlineCurrentSense(.006, 50.0,_NC,22,17); // this is correct setup with c inverted

// IMU and step detection initialization
LSM6DSV_IMU imu;
StepDetector step_detector(15, 1.336, 0.2, 1000);

// position controller initialization
// PositionController p_controller_{1.0, 0.0, 0.0, 12.0};
// PositionController p_controller_{20.0, 0.0, 0.0, 12.0};
PositionController p_controller_{1.0, 0.0, 0.0, 0.0};

TeensyTimerTool::PeriodicTimer command_update_timer_(TeensyTimerTool::TCK);
TeensyTimerTool::PeriodicTimer position_control_timer_(TeensyTimerTool::TCK);
TeensyTimerTool::PeriodicTimer imu_timer_(TeensyTimerTool::TCK);
TeensyTimerTool::PeriodicTimer print_timer_(TeensyTimerTool::TCK);

// init global vars
auto imu_data = accelerations{0.0, 0.0, 0.0};
volatile auto cadence = 0.0;
volatile auto count = 0;
volatile auto target = 0.0;
volatile auto target_angle = 0.0;
volatile auto next_angle = 0.0;
volatile auto system_angle = 0.0;
volatile auto system_vel = 0.0;
auto offset = 0.3;
std::vector<float> trajectory;
char msg[100];

void command_update_loop()
{

  // main FOC algorithm function

  if (count > int(trajectory.size())-1)
  {
    count = 0;
  }

  // pump position controller
  target_angle = trajectory.at(count);
  system_angle =  encoder.getAngle();
  system_vel =  encoder.getVelocity();

  count++;
}

void position_control_loop()
{
  target = p_controller_.pump_controller(target_angle, system_angle, system_vel);
}

void imu_loop()
{
  imu_data = imu.get_acc();
  step_detector.filter_update(imu_data);
  cadence = step_detector.get_cadence();
}

void print_loop()
{
//   sprintf(msg,">setpoint: %f\n>system_pos: %f\n>system_vel: %f\n>error:%f\n>i_target:%f\n",target_angle, system_angle, system_vel,float(target_angle-system_angle), target);
//   Serial.println(msg);
  Serial.print(">setpoint:");
  Serial.println(target_angle, 3);

  Serial.print(">system_pos:");
  Serial.println(system_angle, 3);

  Serial.print(">system_vel:");
  Serial.println(system_vel, 3);

  Serial.print(">error:");
  Serial.println(float(target_angle-system_angle), 3);

  Serial.print(">i_target:");
  Serial.println(target, 3);

  Serial.print(">ax:");
  Serial.println(imu_data.x, 3);

  Serial.print(">ay:");
  Serial.println(imu_data.y, 3);

  Serial.print(">az:");
  Serial.println(imu_data.z, 3);

  Serial.print(">cadence:");
  Serial.println(cadence);
}

void setup() { 
  // init imu
  imu.init();
  
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

  // Example: Setting parameters for the Q axis
  motor.PID_current_q.P = 5.0;      
  motor.PID_current_q.I = 1000.0;   
  motor.LPF_current_q.Tf = 0.005;   

  // D axis parameters should usually match the Q axis
  motor.PID_current_d.P = 5.0;
  motor.PID_current_d.I = 1000.0;
  motor.LPF_current_d.Tf = 0.005;

  // set current limit
  motor.current_limit = 3;

  // use monitoring with serial 
  Serial.begin(115200);

  // comment out if not needed
  motor.useMonitoring(Serial);
  SimpleFOCDebug::enable(&Serial);

  // initialize motor
  motor.voltage_sensor_align = 3.0;
  motor.init();

  // set offsets
//   motor.sensor_direction = Direction::CW;
//   motor.zero_electric_angle = 1.0;

  // align sensor and start FOC
  if(!motor.initFOC()){
      while(1){}
  }

  // set initial motor target
  motor.target = 0.0;

  // get trajectory to follow
  std::copy(step_at_500ms.begin(), step_at_500ms.end(), std::back_inserter(trajectory));
//   std::copy(trajectory_traj_11.begin(), trajectory_traj_11.end(), std::back_inserter(trajectory));

  // enable ffwd
//   p_controller_.set_ffwd_control(true);

  // set offset
  offset = encoder.getAngle();

  // wait 1 second
  delay(1000);

  // start command update loop
  command_update_timer_.begin(
    [](){
      command_update_loop();
    }, 10000);

  // start position controller loop
  position_control_timer_.begin(
    [](){
      position_control_loop();
    }, 1000);

  // start imu loop
  imu_timer_.begin(
    [](){
      imu_loop();
    }, 1ms);

  // start print loop
  print_timer_.begin(
    [](){
      print_loop();
    }, 10000);

}

void loop() { // main FOC algorithm function

  motor.loopFOC();

  motor.move(target);

}