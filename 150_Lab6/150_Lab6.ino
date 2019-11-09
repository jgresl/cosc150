#include <dht.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1

// Create instance of OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Create instance of DHT11 sensor
dht DHT;
 
void setup(){
  Serial.begin(9600);
  
  // Setup OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);
}
 
void loop(){
    DHT.read11(A0);

    // Clear OLED display and set cursor to top left
    display.clearDisplay();
    display.setCursor(0,0);             

    // Display water level
    int waterLevel = analogRead(A1);
    if (waterLevel > 500) {
      display.println("      LOW");
    } else {
      display.println("      HIGH");
    }
    
    display.print("Water: ");
    display.println(waterLevel);


    // Display temperature
    display.print("Temp:  ");
    display.print((int) DHT.temperature);
    display.println((char)247);

    // Display humidity
    display.print("Humid: ");
    display.print((int) DHT.humidity);
    display.println("%");
    
    // Display on OLED   
    display.display();

    // Wait 1 second before accessing sensors
    delay(1000);
}
