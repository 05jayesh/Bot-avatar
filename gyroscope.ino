#include<Wire.h>
#include<Servo.h>
const int gyro=0x68;

int16_t acc_x,acc_y,acc_z,gy_x,gy_y,gy_z,temp;

char tmp_str[7];

char* int_to_str(int16_t i)
{
  sprintf(tmp_str,"%6d",i);
  return tmp_str;
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Wire.begin();
Wire.beginTransmission(gyro);
Wire.write(0x68);
Wire.write(0);
Wire.endTransmission(true);



}

void loop() {
  // put your main code here, to run repeatedly:
Wire.beginTransmission(gyro);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(gyro,7*2,true);

acc_x=Wire.read()<<8|Wire.read();
acc_y=Wire.read()<<8|Wire.read();
acc_z=Wire.read()<<8|Wire.read();
temp=Wire.read()<<8|Wire.read();
gy_x=Wire.read()<<8|Wire.read();
gy_y=Wire.read()<<8|Wire.read();
gy_z=Wire.read()<<8|Wire.read();


Serial.print("aX="); Serial.println(int_to_str(acc_x));
Serial.print(" |aY="); Serial.println(int_to_str(acc_y));
Serial.print(" |aZ="); Serial.println(int_to_str(acc_z));
Serial.print(" |temp="); Serial.println(temp/340.00+36.53);
Serial.print(" |gX="); Serial.println(int_to_str(gy_x));
Serial.print(" |gY="); Serial.println(int_to_str(gy_y));
Serial.print(" |gZ="); Serial.println(int_to_str(gy_z));
Serial.println();

delay(1000);
}
