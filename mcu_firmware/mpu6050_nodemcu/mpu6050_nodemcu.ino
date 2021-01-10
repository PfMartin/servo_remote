#include "Wire.h"

const int MPU_ADDR=0x68;

int16_t accelXRaw,accelYRaw, accelZRaw;
int16_t temperatureRaw, temperature;
int16_t gyroXRaw, gyroYRaw, gyroZRaw;

const int measurements = 2000;

int sumAx, sumAy, sumAz, sumGx, sumGy, sumGz;

int lsbPerG = 16384;
int lsbPerSec = 131;
int offsetAx, offsetAy, offsetAz;
int offsetGx, offsetGy, offsetGz;

char tmp_str[7];
char* convert_int16_to_str(int16_t i){
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}


void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);     // PWR_MGMT_1 register
  Wire.write(0);        //set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  // For changing the sensitivity write value into ACCEL_CONFIG or GYRO_CONFIG register
  // Wire.beginTransmission(MPU_ADDR);
  // Wire.write(0x1C);       // ACCEL_CONFIG register 
  // Wire.write(0);   // set to 00011000 for range of +-16g
  // Wire.endTransmission(true);

  // Calibration
  for (int i; i < measurements; i++) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);     // Starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions 4.2 p.40]
    Wire.endTransmission(false);    // The Parameter indicates that the ESP32 will send a restart. As a result, the connection is kept alive
    Wire.requestFrom(MPU_ADDR, 7*2, true);    // Request a total of 7*2=14 registers    
    
    // Accelerometer data
    // Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
    accelXRaw = Wire.read()<<8 | Wire.read();      // Reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
    accelYRaw = Wire.read()<<8 | Wire.read();      // Reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
    accelZRaw = Wire.read()<<8 | Wire.read();      // Reading registers: 0x3F (ACCEL_ZOUT_H) and 0x3G (ACCEL_ZOUT_L)
    // Temperature data
    temperature = Wire.read()<<8 | Wire.read(); // Reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
    // Gyroscope data 
    gyroXRaw = Wire.read()<<8 | Wire.read();       // Reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
    gyroYRaw = Wire.read()<<8 | Wire.read();       // Reading registers: 0x45 (GYRO_XOUT_H) and 0x46 (GYRO_XOUT_L)
    gyroZRaw = Wire.read()<<8 | Wire.read();       // Reading registers: 0x47 (GYRO_XOUT_H) and 0x48 (GYRO_XOUT_L)
    
    Wire.endTransmission();

    sumAx += accelXRaw;
    sumAy += accelYRaw;
    sumAz += accelZRaw;
    sumGx += gyroXRaw;
    sumGy += gyroYRaw;
    sumGz += gyroZRaw;
  }

  // Average
  offsetAx = sumAx / measurements;
  offsetAy = sumAy / measurements;
  offsetAz = sumAz / measurements;
  offsetGx = sumGx / measurements;
  offsetGy = sumGy / measurements;
  offsetGz = sumGz / measurements;

  Serial.println("");
  Serial.print("Offsets for ");
  Serial.print(measurements);
  Serial.print(":");
  // Serial.println(lsbPerG);
  // Serial.println(lsbPerSec);
  Serial.println(offsetAx);
  Serial.println(offsetAy);
  Serial.println(offsetAz);
  Serial.println(offsetGx);
  Serial.println(offsetGy);
  Serial.println(offsetGz);

  delay(5000);
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);     // Starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions 4.2 p.40]
  Wire.endTransmission(false);    // The Parameter indicates that the ESP32 will send a restart. As a result, the connection is kept alive
  Wire.requestFrom(MPU_ADDR, 7*2, true);    // Request a total of 7*2=14 registers    
  
  // Accelerometer data
  // Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelXRaw = Wire.read()<<8 | Wire.read();      // Reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelYRaw = Wire.read()<<8 | Wire.read();      // Reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelZRaw = Wire.read()<<8 | Wire.read();      // Reading registers: 0x3F (ACCEL_ZOUT_H) and 0x3G (ACCEL_ZOUT_L)
  // Temperature data
  temperatureRaw = Wire.read()<<8 | Wire.read(); // Reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  // Gyroscope data 
  gyroXRaw = Wire.read()<<8 | Wire.read();       // Reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyroYRaw = Wire.read()<<8 | Wire.read();       // Reading registers: 0x45 (GYRO_XOUT_H) and 0x46 (GYRO_XOUT_L)
  gyroZRaw = Wire.read()<<8 | Wire.read();       // Reading registers: 0x47 (GYRO_XOUT_H) and 0x48 (GYRO_XOUT_L)

  float accelX = (accelXRaw - offsetAx);
  float accelY = accelYRaw - offsetAy;
  float accelZ = (accelZRaw - offsetAz + lsbPerG) / 16;

  int16_t gyroX = gyroXRaw - offsetGx;
  int16_t gyroY = gyroYRaw - offsetGy;
  int16_t gyroZ = gyroZRaw - offsetGz;
  
  // Print accelerometer data
  //Serial.print("aX ");
  //Serial.println(convert_int16_to_str(accelX));
  // delay(3);
  // Serial.print("aY ");
  // Serial.println(convert_int16_to_str(accelY));
  // delay(3);
  //Serial.print("aZ ");
  Serial.println(convert_int16_to_str(accelZ));
  //delay(3);
  
  // Print gyroscope data
  //Serial.print("gX ");
  //Serial.println(convert_int16_to_str(gyroX));
  //delay(3);
  //Serial.print("gY ");
  //Serial.println(convert_int16_to_str(gyroY));
  //delay(3);
  //Serial.print("gZ ");
  //Serial.println(convert_int16_to_str(gyroZ));
  
  Wire.endTransmission();
}
