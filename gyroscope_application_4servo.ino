// (c) Michael Schoeffler 2017, http://www.mschoeffler.de

#include "Wire.h" // This library allows you to communicate with I2C devices.
#include <Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;// create servo object to control a servo
// twelve servo objects can be created on most boards

int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.

int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data

int aX,aY,aZ,temp,gX,gY,gZ;

char tmp_str[7]; // temporary variable used in convert function

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

void setup() {
    myservo1.attach(9);
  myservo2.attach(10);
  myservo3.attach(5);
  myservo4.attach(6);
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}
void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)


  //conversion
  aX = (accelerometer_x / 16384.0)*90;
  aY = (accelerometer_y / 16384.0)*90; 
  aZ = (accelerometer_z / 16384.0)*90;

  temp=(temperature/340.00+36.53);

  gX = gyro_x / 131.0;
  gY = gyro_y / 131.0; 
  gZ = gyro_z / 131.0;

  
  // print out data
  Serial.print("|##| position(deg) | aX = "); Serial.print(aX);
  Serial.print(" | aY = "); Serial.print(aY);
  Serial.print(" | aZ = "); Serial.print(aZ);
  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  Serial.print("  |##|  gyro(deg/sec) | gX = "); Serial.print(gX);
  Serial.print(" | gY = "); Serial.print(gY);
  Serial.print(" | gZ = "); Serial.print(gZ);
  Serial.print("  |##|   | temp(deg C) = "); Serial.print(temp);
  Serial.println();


  if(aX>8)
  {
    myservo1.write(pos1+5);
    pos1=pos1+5;
    }


   if(aX<-8)
  {
    myservo2.write(pos2+5);
    pos2=pos2+5;
    }

    if(aY>8)
  {
    myservo3.write(pos3+5);
    pos3=pos3+5;
    }


   if(aY<-8)
  {
    myservo4.write(pos4+5);
    pos4=pos4+5;
    }

    if (temp>27.4)
    {
      pos5=pos5+5;
      myservo1.write(pos5);
      myservo1.write(pos5);
      myservo1.write(pos5);
      myservo1.write(pos5);
     
    }
  // delay
  delay(100);


}
