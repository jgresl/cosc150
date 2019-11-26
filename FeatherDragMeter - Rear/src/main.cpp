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

/* Create an instance of the real time clock */
RTCZero rtc;

/* Create instance of the radio driver */
RH_RF95 rf95(RF95_CHIP_SELECT_PIN, RF95_INTERRUPT_PIN);

/* Gloabal variables */
float batteryVoltage;
int batteryPercent;

void setup()
{
  /* Initialize the real time clock */
  rtc.begin();

  /* Initialize LoRa radio with defined configurations */
  rf95.init();
  rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128); 
}

void loop() {
  uint8_t totalSeconds = rtc.getHours() * 3600 + rtc.getMinutes() * 60 + rtc.getSeconds();
  Serial.println("Sending revolutions to display");
  uint8_t data[] = "hELLO";
  rf95.send(data, sizeof(data));

  /* Delay to prevent loop from running more than once per a second */
  delay(1000);
}