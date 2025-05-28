#include <SimpleFOC.h>
#include <math.h>
#define MAX_SIN_LENGTH 2000
#define CURRENT_RECORDING_LENGTH 10000
#define TRIG 2
// BLDCMotor(pole pair number, phase resistance (optional) );
BLDCMotor motor = BLDCMotor(14,4.5);
// BLDCDriver3PWM(pwmA, pwmB, pwmC, Enable(optional));
BLDCDriver3PWM driver = BLDCDriver3PWM(4,5,6,3);
// MagneticSensorSPI(int cs, float _cpr, int _angle_register)
MagneticSensorSPI encoder = MagneticSensorSPI(10, 14);

InlineCurrentSense current_sense  = InlineCurrentSense(.01, 50.0,_NC,22,18); // this is correct setup with c inverted
// float elec_angle[CURRENT_RECORDING_LENGTH];
// PhaseCurrent_s phase_currents[CURRENT_RECORDING_LENGTH];
// float DC_currents[CURRENT_RECORDING_LENGTH];
DQCurrent_s dq_currents[CURRENT_RECORDING_LENGTH];
float sp_angle[CURRENT_RECORDING_LENGTH];
float ang[CURRENT_RECORDING_LENGTH];
float sp_velocity[CURRENT_RECORDING_LENGTH];
float vel[CURRENT_RECORDING_LENGTH];
float sp_cur[CURRENT_RECORDING_LENGTH];
Commander command = Commander(Serial);
void doMotor(char* cmd) { command.motor(&motor, cmd); }

void setup() { 
  
  // pin trigger
//   pinMode(TRIG,OUTPUT);
//   analogWrite(TRIG, 255/2);
//   digitalWrite(TRIG, LOW);

  
  // initialize encoder sensor hardware
  encoder.init();
  // link the motor to the sensor
  motor.linkSensor(&encoder);

  // driver config
  // power supply voltage [V]
  driver.voltage_power_supply = 11.1;
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
  motor.controller = MotionControlType::angle;
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;

  motor.PID_current_q.P = 0.05;
  motor.PID_current_q.I=  10;
  motor.PID_current_d.P= .05;
  motor.PID_current_d.I = 10;
  motor.LPF_current_q.Tf = 0.01; 
  motor.LPF_current_d.Tf = 0.01; 

  motor.PID_velocity.P = 1; 
  motor.PID_velocity.I = 0;
  motor.PID_velocity.D = 0; 
  motor.PID_velocity.output_ramp = 1000;
  motor.LPF_velocity.Tf = 0.01;
  motor.P_angle.P = 20;

  motor.current_limit = 3;

  // use monitoring with serial 
  Serial.begin(921600);
  // comment out if not needed
  motor.useMonitoring(Serial);
  
  // initialize motor
  motor.init();
  current_sense.skip_align = true;
  // motor.voltage_sensor_align = 3;
  current_sense.gain_b = 1/50.0/0.01;
  current_sense.gain_c = -1/50.0/0.01;
  SimpleFOCDebug::enable(&Serial);
  Serial.printf("gain_a %f, gain_b %f, gain_c %f\n",current_sense.gain_a, current_sense.gain_b,current_sense.gain_c);
  Serial.printf("ia: %f, ib: %f, ic: %f\n",current_sense.offset_ia,current_sense.offset_ib,current_sense.offset_ic);

  // align sensor and start FOC
  if(!motor.initFOC()){
      //while(1){}
  }
  motor.target = 3.14;
  command.add('M', doMotor, "Motor");

  // Create the initial sin array
  delay(1000);
}

void loop() {
    static int c = 0;
    static float targ = 3.14;
    int start = micros();
    if (c == CURRENT_RECORDING_LENGTH/2){
      targ = 0;
    }
    motor.loopFOC();
    motor.move(targ);
    // command.run();
    // phase_currents[c] = current_sense.getPhaseCurrents();
    // elec_angle[c] = motor.electrical_angle;
    // DC_currents[c] = current_sense.getDCCurrent(motor.electrical_angle);

    dq_currents[c] = current_sense.getFOCCurrents(motor.electrical_angle);
    sp_cur[c] = motor.current_sp;
    sp_angle[c] = motor.shaft_angle_sp;
    ang[c] = motor.shaft_angle;
    sp_velocity[c] = motor.shaft_velocity_sp;
    vel[c] = motor.shaft_velocity;
    c++;    // Increment counter


    // Check to loop back counter
    if (c >= CURRENT_RECORDING_LENGTH) {
        c = 0;
        Serial.printf("123412341234,.1,.1,.1,.1,.1,.1,.1\n");

        for(int i = 0; i < CURRENT_RECORDING_LENGTH; i++){
            Serial.printf("%d,%f,%f,%f,%f,%f,%f,%f\n",i,sp_angle[i],ang[i],sp_velocity[i],vel[i],sp_cur[i],dq_currents[i].d,dq_currents[i].q);
        }
        _delay(1);
        Serial.printf("123412341234,.1,.1,.1,.1,.1,.1,.1\n");
        targ  = 3.14;
        _delay(1000);
        

    }

    while(micros()-start < 150) {} // do nothing 


}