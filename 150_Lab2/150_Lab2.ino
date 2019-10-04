#define VEHICLE_RED 10
#define VEHICLE_AMBER 9
#define VEHICLE_GREEN 8
#define PEDESTRIAN_GREEN 2
#define PEDESTRIAN_RED 3
#define BUTTON_PIN 5

// Button - default off
int buttonState = LOW;

void setup() {
  Serial.begin(9600);
    
  // Vehicle red light
  pinMode (VEHICLE_RED, OUTPUT);

  // Vehicle amber light
  pinMode (VEHICLE_AMBER, OUTPUT);

  // Vehicle green light
  pinMode (VEHICLE_GREEN, OUTPUT);

  // Pedestrian green light
  pinMode (PEDESTRIAN_GREEN, OUTPUT);

   // Pedestrian red light
  pinMode (PEDESTRIAN_RED, OUTPUT);
}

 
void loop() {
  // Read the state of the pushbutton value:
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {
    // Vehicle light = green
    digitalWrite (VEHICLE_GREEN, HIGH);
    digitalWrite (VEHICLE_AMBER, LOW);
    digitalWrite (VEHICLE_RED, LOW);

    // Pedestrian light = red
    digitalWrite (PEDESTRIAN_GREEN, LOW);
    digitalWrite (PEDESTRIAN_RED, HIGH);
    
  } else if (buttonState == HIGH) {
    // Vehicle light = amber for 3 seconds
    digitalWrite (VEHICLE_GREEN, LOW);
    digitalWrite (VEHICLE_AMBER, HIGH);
    digitalWrite (VEHICLE_RED, LOW);  
    delay(3000);
    
    // Vehicle light = red
    digitalWrite (VEHICLE_GREEN, LOW);
    digitalWrite (VEHICLE_AMBER, LOW);
    digitalWrite (VEHICLE_RED, HIGH); 

    // Pedestrian light = green
    digitalWrite (PEDESTRIAN_GREEN, HIGH);
    digitalWrite (PEDESTRIAN_RED, LOW);

    // Wait 10 seconds and reset traffic light
    delay(10000);
    buttonState = HIGH;
  } 
}
