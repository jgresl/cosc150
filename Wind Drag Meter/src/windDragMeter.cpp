#include<Arduino.h>
#include<Wire.h>

const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

// RGB LED
int red_light_pin= 13;
int green_light_pin = 12;
int blue_light_pin = 11;

int minVal=265;
int maxVal=402;

double x;
double y;
double z;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);

  // RGB LED
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  
  // Determine angles
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  int xAng = map(AcX,minVal,maxVal,-90,90);
  int yAng = map(AcY,minVal,maxVal,-90,90);
  int zAng = map(AcZ,minVal,maxVal,-90,90);
  x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

  // Print angles
  Serial.print("AngleX= ");
  Serial.println(x);
  //Serial.print("AngleY=   ");
  //Serial.println(y);
  //Serial.print("AngleZ= ");
  //Serial.println(z);

  // Change RGB LED colors based on angles
  analogWrite(red_light_pin, map(x, 0, 360, 0, 255));
  analogWrite(green_light_pin, map(y, 0, 360, 0, 255));
  analogWrite(blue_light_pin, map(z, 0, 360, 0, 255));

  Serial.println("-----------------------------------------");
  delay(1000);
}