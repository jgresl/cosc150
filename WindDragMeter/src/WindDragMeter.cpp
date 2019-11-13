#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SparkFunMPU9250-DMP.h>
#include <RadioHead.h>

#define SCREEN_WIDTH  128   // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define BUTTON_PIN    10    // Button for changing screens
#define OLED_RESET    -1    // No reset pin on OLED display
#define VBATPIN       A7    // Voltage pin to determine battery life

// Declare SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Declare MPU-9250 IMU device (gyrometer, accelerometer, and magnometer)
MPU9250_DMP imu;

// Declare main functions of program
void printWindDrag();
void printCrankPower();
void printCrankCadence();
void printWheelCadence();
void printGroundSpeed();
void printWindSpeed();
void printTemperature();
void printAirPressure();
void printAltitude();
void printSlope();
void printCompassHeading();
void printTimeElapsed();
void printBatteryLife();

// Declare raw data functions
void printRawDataScreen();
void printAccelerometer();
void printGyroscope();
void printMagnetometer();

// Declare global variables
int screen = 0;
float accelX, accelY, accelZ;
float gyroX, gyroY, gyroZ;
float magX, magY, magZ;
float batteryPrevious;
bool isCharging;
int sampleCount, transmissionCount;

void setup() {
  Serial.begin(115200);

  // Initialize menu button
  pinMode(BUTTON_PIN, INPUT);

  // Initialize the OLED display and set default text poroperties
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.setTextSize(2);
  display.setTextColor(WHITE);

  // Initialize the MPU-9250 IMU device (gyrometer, accelerometer, and magnometer)
  if (imu.begin() != INV_SUCCESS) {
    while (true) {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }

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

  // Calculate starting battery life
  batteryPrevious = analogRead(VBATPIN);
  batteryPrevious *= 2;    // Adafruit divided by 2, so multiply back
  batteryPrevious *= 3.3;  // Multiply by 3.3V, the reference voltage
  batteryPrevious /= 1024; // Convert to voltage
}

void loop() {
  if(digitalRead(BUTTON_PIN) == LOW)
    screen++;

  switch(screen) {
    case 0:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printWindDrag();
      break;
    case 1:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printCrankPower();
      break;
    case 2:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printCrankCadence();
      break;
    case 3:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printWheelCadence();
      break;
    case 4:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printGroundSpeed();
      break;
    case 5:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printWindSpeed();
      break;
    case 6:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printTemperature();
      break;
    case 7:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printAirPressure();
      break;
    case 8:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printAltitude();
      break;
    case 9:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printSlope();
      break;
    case 10:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printCompassHeading();
      break;
    case 11:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printTimeElapsed();
      break;
    case 12:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printBatteryLife();
      break;
    case 13:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printRawDataScreen();
      break;
    case 14:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printAccelerometer();
      break;
    case 15:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printGyroscope();
      break;
    case 16:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printMagnetometer();
      break;
    case 17:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      screen = 0;
      break;
  }
}

void printWindDrag() {
  // Display velocity on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Wind Drag");
  display.println();
  display.println("0.00 CdA");
  display.display();
}

void printCrankPower() {
  // Display velocity on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Power");
  display.println();
  display.println("0 watts");
  display.display();
}

void printCrankCadence() {
  // Display velocity on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Crank RPM");
  display.println();
  display.println("0 rpm");
  display.display();
}

void printWheelCadence() {
  // Display velocity on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Wheel RPM");
  display.println();
  display.println("0 rpm");
  display.display();
}

void printGroundSpeed() {
  // Display velocity on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Speed");
  display.println();
  display.println("0.0 km/h");
  display.display();
}

void printWindSpeed() {
  // Display velocity on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Wind Speed");
  display.println();
  display.println("0.0 km/h");
  display.display();
}

void printTemperature() {
  // Display weather on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Temp");
  display.println();
  display.print("21");
  display.print((char) 247);
  display.print("C  ");
  display.print("70");
  display.print((char) 247);
  display.println("F");
  display.display();
}

void printAirPressure() {
  // Display velocity on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Pressure");
  display.println();
  display.println("0 hPa");
  display.display();
}

void printAltitude() {
  // Display velocity on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Altitude");
  display.println();
  display.println("0 m");
  display.display();
}

void printSlope() {
  // Get values from acceleromter
  if (imu.dataReady()) {
    imu.update(UPDATE_ACCEL);
  }

  // Calculate values to (g)
  accelX = imu.calcAccel(imu.ax);
  accelY = imu.calcAccel(imu.ay);
  accelZ = imu.calcAccel(imu.az);

  // Display values on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Slope");
  display.println();
  display.print((int) (accelX * 90));
  display.print((char) 247);
  display.display();
}

void printCompassHeading() {
  // Get values from magnetometer (compass)
  if (imu.dataReady()) {
    imu.update(UPDATE_COMPASS);
  }

  // Convert values to (uT)
  magX = imu.calcMag(imu.mx);
  magY = imu.calcMag(imu.my);
  magZ = imu.calcMag(imu.mz);

  // Calculate 0-360 degree angle
  float angle = (atan2(imu.my, imu.mx) * 180) / PI;
  angle = map(angle, 9, 69, 0, 360);
  int degrees = constrain(((int) angle + 360) % 360, 0, 360);

  // Determine heading
  String heading;
  if (degrees < 33.75)
    heading = "N";
  else if (degrees < 67.5)
    heading = "NE";
  else if (degrees < 112.5)
    heading = "E";
  else if (degrees < 157.5)
    heading = "SE";
  else if (degrees < 202.5)
    heading = "S";
  else if (degrees < 247.5)
    heading = "SW";
  else if (degrees < 292.5)
    heading = "W";
  else if (degrees < 337.25)
    heading = "NW";
   else
    heading = "N";

  // Display values on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Heading");
  display.println();
  display.print(degrees);
  display.print((char)247);
  display.print(" " + heading);
  display.display();

  delay(200);
}

void printTimeElapsed() {
  // Get time elapsed in milliseconds
  int seconds = millis()/1000;

  // Convert time elapsed into hh mm ss
  int hh = (int) (seconds / 3600);
  int mm = ((int) (seconds / 60)) % 60;
  int ss = seconds % 60;

  // Display time elapsed on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Elapsed");
  display.println();
  display.print(hh);
  display.print("h ");
  display.print(mm);
  display.print("m ");
  display.print(ss);
  display.println("s ");
  display.display();
}

void printBatteryLife() {
  // Calculate current battery life
  float batteryNow = analogRead(VBATPIN);
  batteryNow *= 2;    // Adafruit divided by 2, so multiply back
  batteryNow *= 3.3;  // Multiply by 3.3V, the reference voltage
  batteryNow /= 1024; // Convert to voltage
  float batteryPercent = constrain(map(batteryNow * 100, 310, 420, 0, 100), 0 , 100);

  if (batteryNow > batteryPrevious + 0.01) {
    isCharging = true;
  } else if (batteryNow < batteryPrevious - 0.015) {
    isCharging = false;
  }

  // Display battery life on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Battery");
  display.print(batteryNow);
  display.println(" v");
  display.print((int) batteryPercent);
  display.println("%");
  if (isCharging) {
    display.print("Charging");
  } 
  display.display();
  delay(1000);

  // Store current battery life
  batteryPrevious = batteryNow;
}

void printRawDataScreen() {
  // Display battery life on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println();
  display.println("Raw Data");
  display.println("Screens");
  display.display();
}

void printAccelerometer() {
  // Get values from acceleromter
  if (imu.dataReady()) {
    imu.update(UPDATE_ACCEL);
  }

  // Calculate values to (g)
  accelX = imu.calcAccel(imu.ax);
  accelY = imu.calcAccel(imu.ay);
  accelZ = imu.calcAccel(imu.az);

  // Display values on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Accel: g");
  display.print("x: ");
  display.println(accelX);
  display.print("y: ");
  display.println(accelY);
  display.print("z: ");
  display.print(accelZ);
  display.display();
}

void printGyroscope() {
  // Get values from gyroscope
  if (imu.dataReady()) {
    imu.update(UPDATE_GYRO);
  }

  // Convert values to (dps)
  gyroX = imu.calcGyro(imu.gx);
  gyroY = imu.calcGyro(imu.gy);
  gyroZ = imu.calcGyro(imu.gz);

  // Display values on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Gyro: dps");
  display.print("x: ");
  display.println(gyroX);
  display.print("y: ");
  display.println(gyroY);
  display.print("z: ");
  display.print(gyroZ);
  display.display();
}

void printMagnetometer() {
  // Get values from magnetometer (compass)
  if (imu.dataReady()) {
    imu.update(UPDATE_COMPASS);
  }

  // Convert values to (uT)
  magX = imu.calcMag(imu.mx);
  magY = imu.calcMag(imu.my);
  magZ = imu.calcMag(imu.mz);

  // Display values on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Magnet: uT");
  display.print("x: ");
  display.println(magX);
  display.print("y: ");
  display.println(magY);
  display.print("z: ");
  display.print(magZ);
  display.display();
}