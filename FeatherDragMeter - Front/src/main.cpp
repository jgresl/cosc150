#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RH_RF95.h>
#include <rtcZero.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SparkFunMPU9250-DMP.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Feather M0 TFT display wiring configurations
#define STMPE_CS 6
#define TFT_CS 9
#define TFT_DC 10
#define SD_CS 5

// Feather M0 wiring configurations
#define RF95_CHIP_SELECT_PIN 8
#define RF95_INTERRUPT_PIN 3
#define RF95_RESET_PIN 4

// LoRa radio configurations
#define RF95_FREQUENCY 915.0         // Between 137.0 and 1020.0   (Default = 915 Mhz)
#define RF95_TRANSMISSION_POWER 5    // Between 5 and 23           (Default = 13 Db)

// Declare functions
void printSensorHeaders();
void printSensorValues();
int getTemperature();
float getPressure();
int getAltitude();
int getSlope();
int getAirSpeed();
float getHumidity();
int getRPM();
void getElapsed();
float getDistance();
int getCDA();
int getVelocity();
int getPower();

// Create instance of TFT display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Create an instance of the real time clock
RTCZero rtc;

// Create instance of the radio driver
RH_RF95 rf95(RF95_CHIP_SELECT_PIN, RF95_INTERRUPT_PIN);

// Create instance of BME280 sensor
Adafruit_BME280 bme;

// Declare MPU-9250 IMU device (gyrometer, accelerometer, and magnometer)
MPU9250_DMP imu;

// Gloabal variables
float wheelCircumference = 0.00179;
float currentRevolutions = 0, prevRevolutions1 = 0, prevRevolutions2 = 0, prevRevolutions3 = 0, prevRevolutions4 = 0, prevRevolutions5 = 0;

// float V_0 = 5.0; 
// float rho = 1.204; 
// int offset = 0;
// int offset_size = 10;
// int veloc_mean_size = 20;
// int zero_span = 2;

void setup() {
  // Initialize the real time clock
  rtc.begin();

  // Begin TFT display
  tft.begin();

  // Initialize LoRa radio with defined configurations
  rf95.init();
  rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128); 

  // Begin BMP280 sensor
  bme.begin(0x76);

  // Begin Airspeed Sensor
  pinMode(A1, INPUT);

  // Initialize the MPU-9250 IMU device (gyrometer, accelerometer, and magnometer)
  imu.begin(); 

  // for (int i = 0; i < offset_size; i++) {
  //   offset += analogRead(A1)-(1023/2);
  // }
  // offset /= offset_size;

  // Enable all MPU-9250 sensors:
  // imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
  imu.setSensors(INV_XYZ_ACCEL);


  // Set the gyroscope full scall range (+/- 250, 500, 1000, or 2000 dps)
  // imu.setGyroFSR(2000);

  // Set the accelerometer full scale range (+/- 2, 4, 8, or 16 g)
  imu.setAccelFSR(2);

  // The magnetometer full scale range is preset to +/- 4912 uT (micro-tesla's)

  // Set the digital low-pass filter of the accelerometer and gyroscope (88, 98, 42, 20, 10, or 5 Hz)
  // imu.setLPF(5);

  // Set the accellerometer and gyroscope sample rate (4Hz to 1kHz)
  // imu.setSampleRate(10);

  // Set the compass sample rate (1Hz to 100Hz)
  // imu.setCompassSampleRate(10);

  // Print sensor headers
  printSensorHeaders();
}

void loop() {
  uint8_t totalSeconds = rtc.getHours() * 3600 + rtc.getMinutes() * 60 + rtc.getSeconds();

  // Get values from acceleromter
  if (imu.dataReady()) {
    imu.update(UPDATE_ACCEL);
  }

  // Get revolutions from rear feather (6 seconds)
  prevRevolutions1 = prevRevolutions2;
  prevRevolutions2 = prevRevolutions3;
  prevRevolutions3 = prevRevolutions4;
  prevRevolutions4 = prevRevolutions5;
  prevRevolutions5 = currentRevolutions;
  currentRevolutions = totalSeconds;

  // if (rf95.available()) {
  //   // Should be a message for us now   
  //   uint8_t buffer[10];
  //   uint8_t length = sizeof(buffer);
  //   if (rf95.recv(buffer, &length)) {
  //     Serial.println((char*) buffer);
  //   }
  // }
  
  // Update and display sensor values
  printSensorValues();
  
  // Delay to prevent loop from running more than once per a second
  delay(1000);
}

void printSensorHeaders() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);
  
  // Horizontal lines
  tft.drawLine(0, 60, 320, 60, ILI9341_GREEN);
  tft.drawLine(0, 120, 320, 120, ILI9341_GREEN);
  tft.drawLine(0, 180, 320, 180, ILI9341_GREEN);
  
  // Vertical lines
  tft.drawLine(80, 0, 80, 240, ILI9341_GREEN);
  tft.drawLine(200, 0, 200, 240, ILI9341_GREEN);

  // Sensor headers
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(10, 5);
  tft.print("Temp   Humidity  Pressure");
  tft.setCursor(10, 65);
  tft.print("cDa    Airspeed  Altitude");
  tft.setCursor(10, 125);
  tft.print("RPM    Velocity  Distance");
  tft.setCursor(10, 185);
  tft.print("Slope  Power     Elapsed");
}

void printSensorValues() {
  tft.setTextColor(ILI9341_WHITE);

  // Temperature
  tft.fillRect(10, 30, 65, 15, ILI9341_BLACK);
  tft.setCursor(10, 30);
  tft.print(getTemperature());
  tft.print((char)247);
  tft.print(" C");

  // Humidity
  tft.fillRect(95, 30, 100, 15, ILI9341_BLACK);
  tft.setCursor(95, 30);
  tft.print(getHumidity());
  tft.print(" %");

  // Pressure
  tft.fillRect(215, 30, 100, 15, ILI9341_BLACK);
  tft.setCursor(215, 30);
  tft.print(getPressure());
  tft.setTextSize(1);
  tft.print(" kPa");
  tft.setTextSize(2);
  
  // cDa
  tft.fillRect(10, 90, 65, 15, ILI9341_BLACK);
  tft.setCursor(10, 90);
  tft.print(getCDA());

  // Air Speed
  tft.fillRect(95, 90, 100, 15, ILI9341_BLACK);
  tft.setCursor(95, 90);
  tft.print(getAirSpeed());
  tft.print(" m/s");
  
  // Altitude
  tft.fillRect(215, 90, 100, 15, ILI9341_BLACK);
  tft.setCursor(215, 90);
  tft.print(getAltitude());
  tft.print(" m");
  
  // RPM
  tft.fillRect(10, 150, 65, 15, ILI9341_BLACK);
  tft.setCursor(10, 150);
  tft.print(getRPM());
  
  // Velocity
  tft.fillRect(95, 150, 100, 15, ILI9341_BLACK);
  tft.setCursor(95, 150);
  tft.print(getVelocity());
  tft.print(" km/h");

  // Distance
  tft.fillRect(215, 150, 100, 15, ILI9341_BLACK);
  tft.setCursor(215, 150);
  tft.print(getDistance());
  tft.print(" km");

  // Slope
  tft.fillRect(10, 210, 65, 15, ILI9341_BLACK);
  tft.setCursor(10, 210);
  tft.print(getSlope());
  tft.print((char) 247);

  // Power
  tft.fillRect(95, 210, 100, 15, ILI9341_BLACK);
  tft.setCursor(95, 210);
  tft.print(getPower());
  tft.println(" w");
  
  // Elapsed
  tft.fillRect(215, 210, 100, 15, ILI9341_BLACK);
  tft.setCursor(215, 210);
  getElapsed();
}

// Getter functions
int getTemperature() {
  return bme.readTemperature();
}

float getPressure() {
  float pa = bme.readPressure();
  float kpa = pa / 1000;
  return kpa;
}

int getAltitude() {
  return bme.readAltitude(1013.25);
}

int getCDA() {
  return  0;
}

int getAirSpeed() {
  // float adc_avg = 0;
  // float airSpeed = 0.0;
  
  // for (int ii=0;ii<veloc_mean_size;ii++){
  //   adc_avg+= analogRead(A0)-offset;
  // }
  // adc_avg/=veloc_mean_size;
  
  
  // if (adc_avg>512-zero_span and adc_avg<512+zero_span){
  // } else{
  //   if (adc_avg<512){
  //     airSpeed = -sqrt((-10000.0*((adc_avg/1023.0)-0.5))/rho);
  //   } else{
  //     airSpeed = sqrt((10000.0*((adc_avg/1023.0)-0.5))/rho);
  //   }
  // }

  return 0;
}

float getHumidity() {
  return bme.readHumidity();
}

int getRPM() {
  float rps = (currentRevolutions - prevRevolutions1) / 6;
  return (rps * 60);
}

int getVelocity() {
  float rph = (currentRevolutions - prevRevolutions1) / 6 * 3600;
  return rph * wheelCircumference;
}

float getDistance() {
  return currentRevolutions * wheelCircumference;
}

int getSlope() {
  float accelX = imu.calcAccel(imu.ax);
  int slope = accelX * 90;
  return slope;
}

int getPower() {
  return 0;
}

void getElapsed() {
  if (rtc.getHours() < 10)
    tft.print("0");
  tft.print(rtc.getHours());
  tft.print(":");
  if(rtc.getMinutes() < 10)
    tft.print("0");
  tft.print(rtc.getMinutes());
  tft.print(":");
  if(rtc.getSeconds() < 10)
    tft.print("0");
  tft.print(rtc.getSeconds());
}