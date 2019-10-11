#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define BUTTON_PIN 10

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int screen = 0;

void setup() {
  Serial.begin(115200);

  // Begin OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Set pin 10 to button
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  if(digitalRead(BUTTON_PIN) == LOW)
    screen++;

  switch(screen) {
    case 0:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printTiltAngle();
      break;
    case 1:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printVelocity();
      break;
    case 2:
      while(digitalRead(BUTTON_PIN) == LOW) {}
      printTemperature();
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
      screen = 0;
      break;
  }
 
}

void printTiltAngle() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Tilt Angle");
  display.println("x: 45");
  display.println("y: 15");
  display.println("z: 60");
  display.display();
}

void printVelocity() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Velocity");
  display.println();
  display.println("0 km/h");
  display.display();
}

void printTemperature() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Temperatur");
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
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Battery");
  display.println();
  display.println("82%");
  display.display();
}

void printTimeElapsed() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Elapsed");
  display.println();

  int seconds = millis()/1000;
  int hh = (int) (seconds / 3600);
  int mm = ((int) (seconds / 60)) % 60;
  int ss = seconds % 60;
  
  display.print(hh);
  display.print("h ");
  display.print(mm);
  display.print("m ");
  display.print(ss);
  display.println("s ");
  display.display();
}
