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
float elec_angle[CURRENT_RECORDING_LENGTH];
PhaseCurrent_s phase_currents[CURRENT_RECORDING_LENGTH];
DQCurrent_s dq_currents[CURRENT_RECORDING_LENGTH];
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
  motor.controller = MotionControlType::torque;
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;

  // foc current control parameters (Arduino UNO/Mega)
  // motor.PID_current_q.P = 5;
  // motor.PID_current_q.I= 300;
  // motor.PID_current_d.P= 5;
  // motor.PID_current_d.I = 300;
  // motor.LPF_current_q.Tf = 0.01; 
  // motor.LPF_current_d.Tf = 0.01; 

  motor.PID_current_q.P = .01;
  motor.PID_current_q.I= 300;
  motor.PID_current_d.P= .01;
  motor.PID_current_d.I = 300;
  motor.LPF_current_q.Tf = 0.01; 
  motor.LPF_current_d.Tf = 0.01; 

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
  motor.target = 0.5;
  command.add('M', doMotor, "Motor");

  // Create the initial sin array
  delay(1000);
}

void loop() {
    static int c = 0;

    int start = micros();
    
    motor.loopFOC();
    motor.move(0.5);
    // command.run();
    phase_currents[c] = current_sense.getPhaseCurrents();
    elec_angle[c] = motor.electrical_angle;
    dq_currents[c] = current_sense.getFOCCurrents(motor.electrical_angle);
    // current_sense.getDQCurrents(motor.electrical_angle)
    c++;    // Increment counter


    // Check to loop back counter
    if (c >= CURRENT_RECORDING_LENGTH) {
        c = 0;
        Serial.printf("123412341234,.1,.1,.1,.1,.1,.1\n");

        for(int i = 0; i < CURRENT_RECORDING_LENGTH; i++){

            Serial.printf("%d,%f,%f,%f,%f,%f,%f\n",i,phase_currents[i].a,phase_currents[i].b,phase_currents[i].c,elec_angle[i],dq_currents[i].d,dq_currents[i].q);
            // Serial.printf("%d,%f,%f\n",i,dq_currents[i].q,dq_currents[i].d);
        }
        _delay(1);
        Serial.printf("123412341234,.1,.1,.1,.1,.1,.1\n");

        _delay(1000);
        

    }

    while(micros()-start < 150) {} // do nothing 


}