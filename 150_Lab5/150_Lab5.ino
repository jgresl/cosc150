#include <IRremote.h>
#include <Servo.h>

// Create instance of IR reciever object
IRrecv irrecv(11);

// Create instance of servo object
Servo myservo;

// Declare global variables
int servo_position = 0;
decode_results results;

void setup(){
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  irrecv.enableIRIn(); // Start the receiver
}

void loop(){
  // Print decode value to serial monitor
  if (irrecv.decode(&results) && results.value != 0xFFFFFFFF) {
    Serial.println(results.value, HEX);

    // Switch statement to control servo commands
    switch (results.value) {
      case 0xFF22DD: 
        if (servo_position < 180) {
          Serial.println("Moving from to 0 degrees");
          // Tell servo to go from 0 to 180 degrees
          for (servo_position = servo_position; servo_position < 180; servo_position++) {
            myservo.write(servo_position);              
            delay(15);
          }
          results.value = 0xFFFFFFFF;
        } else {
          Serial.println("Already at 180 degrees");
        }
        break;
      
      case 0xFF02FD:
          if (servo_position > 0) {
            Serial.println("Moving to 0 degrees");
            // Tell servo to go from 180 to 0 degrees
            for (servo_position = servo_position; servo_position > 0; servo_position--) {
              myservo.write(servo_position);
              delay(15);
            }
            results.value = 0xFFFFFFFF; 
          } else {
            Serial.println("Already at 0 degrees");
          }
        break;
      
      case 0xFF6897:
          if (servo_position == 0) {
            Serial.println("Moving from 0 to 90 degrees");
            // Tell servo to go from 180 to 0 degrees
            for (servo_position = 0; servo_position < 90; servo_position++) {
              myservo.write(servo_position);
              delay(15);
            }
            results.value = 0xFFFFFFFF; 
          } else if (servo_position == 180) {
            Serial.println("Moving from 180 to 90 degrees");
            // Tell servo to go from 180 to 0 degrees
            for (servo_position = 180; servo_position > 90; servo_position--) {
              myservo.write(servo_position);
              delay(15);
            }
            results.value = 0xFFFFFFFF;
          } else {
            Serial.println("Already at 0 degrees");
          }
        break;
    }
  }
  
  // Receive the next IR value
  irrecv.resume();
  delay(100);
}
