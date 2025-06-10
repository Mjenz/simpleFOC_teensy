#include <Wire.h>

#define I2C_SDA 18
#define I2C_SCL 19

uint8_t LSM6DS3 = 0x6A; // Default I2C address
uint8_t num_registers = 6;
float AccX, AccY, AccZ;

void setup() {
  Serial.begin(115200);
  Wire.setSCL(I2C_SCL);
  Wire.setSDA(I2C_SDA);
  Wire.begin();

  // Enable accelerometer at 1.66 kHz, ±2g, 100 Hz filter
  Wire.beginTransmission(LSM6DS3);
  Wire.write(0x10);      // CTRL1_XL register
  Wire.write(0x60);      // ODR_XL = 0110 (1.66 kHz), FS_XL = 00 (±2g), BW_XL = 00
  Wire.endTransmission();
  
  delay(100); // Wait for the sensor to stabilize
}

void loop() {
  // Read 6 bytes from OUTX_L_XL (0x28)
  Wire.beginTransmission(LSM6DS3);
  Wire.write(0x28);
  Wire.endTransmission(false);
  Wire.requestFrom(LSM6DS3, num_registers, true);

  int16_t rawX = Wire.read() | (Wire.read() << 8);
  int16_t rawY = Wire.read() | (Wire.read() << 8);
  int16_t rawZ = Wire.read() | (Wire.read() << 8);

  // Sensitivity for ±2g is 0.061 mg/LSB = 0.000061 g/LSB
  AccX = rawX * 0.000061;
  AccY = rawY * 0.000061;
  AccZ = rawZ * 0.000061;

  // Print values
  char ms[50];
  sprintf(ms, ">ax:%.3f\n>ay:%.3f\n>az:%.3f\n", AccX, AccY, AccZ);
  Serial.print(ms);

  delay(10); // small delay
}
