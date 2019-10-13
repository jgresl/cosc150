#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SparkFunMPU9250-DMP.h>

#define SCREEN_WIDTH  128   // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define BUTTON_PIN    10    // Button for changing screens
#define OLED_RESET    -1    // No reset pin on OLED display

// Declare SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Declare MPU-9250 IMU device (gyrometer, accelerometer, and magnometer)
MPU9250_DMP imu;

// Declare all functions used in the program
void printSerialMonitor();
void printVelocity();
void printWeather();
void printBatteryLife();
void printTimeElapsed();
void printAccelerometer();
void printGyroscope();
void printMagnetometer();

// Declare global variables
int screen = 0;
float accelX;
float accelY;
float accelZ;
float gyroX;
float gyroY;
float gyroZ;
float magX;
float magY;
float magZ;

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

  // Set the magnetometer full scale range (+/- 4912 uT-micro-tesla's)

  // Set the digital low-pass filter of the accelerometer and gyroscope (88, 98, 42, 20, 10, or 5 Hz)
  imu.setLPF(5);

  // Set the accellerometer and gyroscope sample rate (4Hz to 1kHz)
  imu.setSampleRate(10);

  // Set the compass sample rate (1Hz to 100Hz)
  imu.setCompassSampleRate(10);
}

void loop() {
  if(digitalRead(BUTTON_PIN) == LOW)
    screen++;

  switch(screen) {
    case 0:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printSerialMonitor();
      break;
    case 1:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printVelocity();
      break;
    case 2:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printWeather();
      break;
    case 3:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printBatteryLife();
      break;
    case 4:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printTimeElapsed();
      break;
    case 5:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printAccelerometer();
      break;
    case 6:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printGyroscope();
      break;
    case 7:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printMagnetometer();
      break;
    case 8:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      screen = 0;
      break;
  }
}

void printSerialMonitor() {
  // Get acceleration, gyro, and magnetism values from sensor
  if (imu.dataReady()) {
    imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
  }
  
  // Calculate acceleration, gyro, and magnetism
  accelX = imu.calcAccel(imu.ax);
  accelY = imu.calcAccel(imu.ay);
  accelZ = imu.calcAccel(imu.az);
  gyroX = imu.calcGyro(imu.gx);
  gyroY = imu.calcGyro(imu.gy);
  gyroZ = imu.calcGyro(imu.gz);
  magX = imu.calcMag(imu.mx);
  magY = imu.calcMag(imu.my);
  magZ = imu.calcMag(imu.mz);

  // Display serial mode on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println();
  display.println("Serial");
  display.println("Monitor");
  display.display();

  // Display values in serial monitor  
  Serial.println("Accel: " + String(accelX) + ", " + String(accelY) + ", " + String(accelZ) + " g");
  Serial.println("Gyro: " + String(gyroX) + ", " + String(gyroY) + ", " + String(gyroZ) + " dps");
  Serial.println("Mag: " + String(magX) + ", " + String(magY) + ", " + String(magZ) + " uT");
  Serial.println("Time: " + String(imu.time) + " ms");
  Serial.println();
}

void printVelocity() {
  // Display velocity on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Speed");
  display.println();
  display.println("0 km/h");
  display.display();
}

void printWeather() {
  // Display weather on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Weather");
  display.println();
  display.print("10");
  display.print((char)247);
  display.print("C  ");
  display.print("52");
  display.print((char)247);
  display.println("F");
  display.display();
}

void printBatteryLife() {
  // Display battery life on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Battery");
  display.println();
  display.println("82%");
  display.display();
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