// #include <Wire.h>
// #include "lsm6dsv_reg.h"

// #define I2C_SDA 18
// #define I2C_SCL 19
// #define LSM6DSV_ADDR 0x6A

// static stmdev_ctx_t dev_ctx;

// /* Raw data */
// static int16_t accel_raw[3];
// static float accel_mg[3];

// /* -------- I2C low-level functions -------- */
// static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len)
// {
//   Wire.beginTransmission(LSM6DSV_ADDR);
//   Wire.write(reg);
//   Wire.write(bufp, len);
//   return Wire.endTransmission();
// }

// static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
// {
//   Wire.beginTransmission(LSM6DSV_ADDR);
//   Wire.write(reg);
//   Wire.endTransmission(false);
//   Wire.requestFrom(LSM6DSV_ADDR, len);
//   for (uint16_t i = 0; i < len; i++) {
//     bufp[i] = Wire.read();
//   }
//   return 0;
// }

// void setup()
// {
//   Serial.begin(115200);

//   Wire.setSDA(I2C_SDA);
//   Wire.setSCL(I2C_SCL);
//   Wire.begin();

//   /* Initialize driver context */
//   dev_ctx.write_reg = platform_write;
//   dev_ctx.read_reg  = platform_read;
//   dev_ctx.handle    = NULL;

//   uint8_t whoami;
//   lsm6dsv_device_id_get(&dev_ctx, &whoami);

//   if (whoami != LSM6DSV_ID) {
//     Serial.println("LSM6DSVTR not found!");
//     while (1);
//   }

//   Serial.println("LSM6DSVTR connected");

//   /* Reset device */
//   lsm6dsv_sh_reset_set(&dev_ctx, PROPERTY_ENABLE);
//   delay(100);
//   lsm6dsv_sh_reset_set(&dev_ctx, &whoami);

//   /* Enable Block Data Update */
//   lsm6dsv_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

//   /* Set accelerometer:
//      - ODR: 1.66 kHz
//      - FS: ±2 g
//   */
//   lsm6dsv_xl_full_scale_set(&dev_ctx, LSM6DSV_2g);
//   lsm6dsv_xl_data_rate_set(&dev_ctx, LSM6DSV_ODR_AT_60Hz);
// }

// void loop()
// {
//   lsm6dsv_acceleration_raw_get(&dev_ctx, accel_raw);

//   accel_mg[0] = lsm6dsv_from_fs2_to_mg(accel_raw[0]);
//   accel_mg[1] = lsm6dsv_from_fs2_to_mg(accel_raw[1]);
//   accel_mg[2] = lsm6dsv_from_fs2_to_mg(accel_raw[2]);

//   Serial.print(">ax:");
//   Serial.println(accel_mg[0] / 1000.0f, 3);

//   Serial.print(">ay:");
//   Serial.println(accel_mg[1] / 1000.0f, 3);

//   Serial.print(">az:");
//   Serial.println(accel_mg[2] / 1000.0f, 3);

//   delay(10);
// }
