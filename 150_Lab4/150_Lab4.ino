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
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);

  if (results.value > 0) {
    // Tell servo to go from 0 degrees to 180 degrees
    for (servo_position = 0; servo_position <= 180; servo_position += 1)
      myservo.write(servo_position);              
      delay(15);
    }

    // Tell servo to go from 180 degrees to 0 degrees
    for (servo_position = 180; servo_position >= 0; servo_position -= 1) {
      myservo.write(servo_position);
      delay(15);
    }
  }
   
  // Receive the next IR value
  irrecv.resume();
  }
  delay(100);
}
