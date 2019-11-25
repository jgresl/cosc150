#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <string.h>
#include <rtcZero.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SparkFunMPU9250-DMP.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;


float V_0 = 5.0; 
float rho = 1.204; 
int offset = 0;
int offset_size = 10;
int veloc_mean_size = 20;
int zero_span = 2;

/* Feather M0 TFT display wiring configurations */
#define STMPE_CS 6
#define TFT_CS 9
#define TFT_DC 10
#define SD_CS 5

/* Feather M0 wiring configurations */
#define RF95_CHIP_SELECT_PIN 8
#define RF95_INTERRUPT_PIN 3
#define RF95_RESET_PIN 4
#define VBATPIN A7 // Voltage pin to determine battery life

/* LoRa radio configurations */
#define RF95_FREQUENCY 915.0         // Between 137.0 and 1020.0   (Default = 915 Mhz)
#define RF95_TRANSMISSION_POWER 20   // Between 5 and 23           (Default = 13 Db)
#define RF95_CAD_TIMEOUT 10000       // Greater or equal to 0      (Default = 0 ms)
#define RF95_SPREADING_FACTOR 12     // Between 6 and 12           (Default = ?)             --> Overwritten by setModemConfig()
#define RF95_BANDWIDTH 125000        // Between 7800 and 500000    (Default = 125000 Hz)     --> Overwritten by setModemConfig()
#define RF95_PREAMBLE_LENGTH 8       //                            (Default = 8)
#define RF95_SYNC_WORD 0x39          //                            (Default = 0x39)
#define RF95_CODING_RATE 5           // Between 5 and 8            (Default = 5-bit)         --> Overwritten by setModemConfig()
#define RF95_RESTRANSMIT_RETRIES 3   //                            (Default = 3)
#define RF95_RESTRANSMIT_TIMEOUT 500 //                            (Default = 200 ms)
#define RF95_GATEWAY_ID 100          // Assign unique ID to gateway

/* Declare functions */
void initialize_radio();
void printSensorValues();
int getTemperature();
int getPressure();
int getAltitude();
int getSlope();
int getAirSpeed();
int getHumidity();
int getTime();
int getElapsed();
int getCadence();
int getCDA();
int getVelocity();
int getPower();

/* Create instance of TFT display */
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

/* Create an instance of the real time clock */
RTCZero rtc;

/* Create instance of the radio driver */
RH_RF95 rf95(RF95_CHIP_SELECT_PIN, RF95_INTERRUPT_PIN);

/* Create instance of the packet manager */
//RHReliableDatagram manager(rf95);

/* Create instance of BME280 sensor */
//Adafruit_BMP280 bmp;

// Declare MPU-9250 IMU device (gyrometer, accelerometer, and magnometer)
MPU9250_DMP imu;

/* Gloabal variables */
float batteryVoltage;
int batteryPercent;

void setup()
{
  /* Initialize the real time clock */
  rtc.begin();

  /* Begin TFT display */
  tft.begin();

  /* Initialize LoRa radio with defined configurations */
  initialize_radio();

  /* Begin BMP280 sensor */
  //bmp.begin(0x76);

  /* Begin Airspeed Sensor */
  pinMode(A0, INPUT);

  // Initialize the MPU-9250 IMU device (gyrometer, accelerometer, and magnometer)
  if (imu.begin() != INV_SUCCESS) {
    while (true) {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  if(!bme.begin(0x76)){
      Serial.println("Could not find a valid BME280 Sensor, check wiring!");
      while(1);
  }
  for (int ii=0;ii<offset_size;ii++){
    offset += analogRead(A0)-(1023/2);
  }
  offset /= offset_size;

  // Enable all MPU-9250 sensors:
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);

  // Set the gyroscope full scall range (+/- 250, 500, 1000, or 2000 dps)
  imu.setGyroFSR(2000);

  // Set the accelerometer full scale range (+/- 2, 4, 8, or 16 g)
  imu.setAccelFSR(2);

  // The magnetometer full scale range is preset to +/- 4912 uT (micro-tesla's)

  // Set the digital low-pass filter of the accelerometer and gyroscope (88, 98, 42, 20, 10, or 5 Hz)
  imu.setLPF(5);

  // Set the accellerometer and gyroscope sample rate (4Hz to 1kHz)
  imu.setSampleRate(10);

  // Set the compass sample rate (1Hz to 100Hz)
  imu.setCompassSampleRate(10);
}
}

void loop()
{
  uint8_t totalSeconds = rtc.getHours() * 3600 + rtc.getMinutes() * 60 + rtc.getSeconds();

  // Get values from acceleromter
  if (imu.dataReady()) {
    imu.update(UPDATE_ACCEL);
  }

  /* Update display every 5 seconds */
  if (totalSeconds % 1 == 0)
  {
    printSensorValues();
  }

  /* Delay to prevent loop from running more than once per a second */
  delay(1000);
}

void initialize_radio()
{
  /* Manual reset on radio module */
  pinMode(RF95_RESET_PIN, OUTPUT);
  digitalWrite(RF95_RESET_PIN, LOW);
  delay(10);
  digitalWrite(RF95_RESET_PIN, HIGH);
  delay(10);
  void print_setup();
}

void printSensorValues()
{
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  
  // Horizontal lines
  tft.drawLine(0, 60, 320, 60, ILI9341_GREEN);
  tft.drawLine(0, 120, 320, 120, ILI9341_GREEN);
  tft.drawLine(0, 180, 320, 180, ILI9341_GREEN);
  
  // Vertical lines
  tft.drawLine(80, 0, 80, 240, ILI9341_GREEN);
  tft.drawLine(200, 0, 200, 240, ILI9341_GREEN);
  
  tft.setCursor(0, 0);
  tft.setTextSize(2);

  // Line 1
  tft.setTextColor(ILI9341_GREEN);
  tft.println(" Temp   Pressure  Altitude\n");
  tft.setTextColor(ILI9341_WHITE);
  tft.print(" ");
  tft.print(getTemperature());
  tft.print((char)247);
  tft.print("C   ");
  tft.print(getPressure());
  tft.print(" Pa  ");
  tft.print(getAltitude());
  tft.print(" m\n\n");
  
  // Line 2
  tft.setTextColor(ILI9341_GREEN);
  tft.println(" Slope  Airspeed  Humidity\n");
  tft.setTextColor(ILI9341_WHITE);
  tft.print(" ");
  tft.print(getSlope());
  tft.print((char) 247);
  tft.print("    ");
  tft.print(getAirSpeed());
  tft.print("m/s");
  tft.print("     ");
  tft.print(getHumidity());
  tft.println("%\n");

  // Line 3
  tft.setTextColor(ILI9341_GREEN);
  tft.println(" Time   Elapsed   Cadence\n");
  tft.setTextColor(ILI9341_WHITE);
  tft.print(" ");
  tft.print(getTime());
  tft.print((char) 247);
  tft.print("    ");
  tft.print(getAirSpeed());
  tft.print(" km/h");
  tft.print("   ");
  tft.print(getCadence());
  tft.println(" rpm\n");

  // Line 4
  tft.setTextColor(ILI9341_GREEN);
  tft.println(" cDa    Velocity  Power\n");
  tft.setTextColor(ILI9341_WHITE);
  tft.print(" ");
  tft.print(getCDA());
  tft.print((char) 247);
  tft.print("    ");
  tft.print(getVelocity());
  tft.print(" km/h");
  tft.print("    ");
  tft.print(getPower());
  tft.println(" w");
}

int getTemperature() {
  return bme.readTemperature();
}

int getPressure() {
  return bme,readPressure()/100;
}

int getAltitude() {
  return 530;
}

int getSlope() {
  int accelX = imu.calcAccel(imu.ax);
  int slope = accelX * 90;
  return slope;
}

int getAirSpeed() {
  
  float adc_avg = 0; float veloc = 0.0;
  
  for (int ii=0;ii<veloc_mean_size;ii++){
    adc_avg+= analogRead(A0)-offset;
  }
  adc_avg/=veloc_mean_size;
  
  
  if (adc_avg>512-zero_span and adc_avg<512+zero_span){
  } else{
    if (adc_avg<512){
      veloc = -sqrt((-10000.0*((adc_avg/1023.0)-0.5))/rho);
    } else{
      veloc = sqrt((10000.0*((adc_avg/1023.0)-0.5))/rho);
    }
  }
  return veloc;
}

int getHumidity() {
  return bme.readHumidity();
}

int getTime() {
  return 1;
}

int getElapsed() {
  int elapsed = rtc.getSeconds();
  return elapsed;
}

int getCadence() {
  return 60;
}

int getCDA() {
  return  50;
}

int getVelocity() {
  return 30;
}

int getPower() {
  return 600;
}