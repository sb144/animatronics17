#include <Servo.h>

/*
 * TSA Animatronics
 * Rock Canyon High School :: 2035-1
 * Sam Basile, Zain Elsell, Jackson Willner, and Vishal Nallam
 */

//Digital output pins for the servos
unsigned const int headPin = 5, jawPin = 9, tailPin = 11, echoPin = 6, trigPin= 7;

//Servo control objects
Servo head, jaw, tail;

//First ultrasonic distance
long maximumDistance = 0;

//The setup method is run when the Arduino gets power
void setup() {
  //Start the serial connection
  Serial.begin(9600);
  
  //Set the servo objects to output to their respective pins
  head.attach(headPin);
  jaw.attach(jawPin);
  tail.attach(tailPin);

  //Set pins up for the ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

//The loop method is continously called as long as the Arduino has power
void loop() {
  //Give a grace period for anything incorrect to correct itself
  delay(800);

  //Read ultrasonic values
  readUltrasonic();

  //Very small delay to allow for the method to run
  delay(10);

  //Start off by moving the dinosaur's head
  turnHead();

  //Small delay to allow for full completion
  delay(150);

  //Wag the tail twice
  wagTail();
  wagTail();

  //Small delay to allow for full completion
  delay(200);

  //Finish off by opening and closing the jaw
  useJaw();
}

//Send out an ping, read value and assess the proper actions based upon the response
void readUltrasonic() {
  //Send out a ping
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //Retrieve the response
  long distance = pulseIn(echoPin, HIGH) / 74 / 2;
  if(maximumDistance == 0) {
    //If this is the first ping, set the initial distance
    maximumDistance = distance;
    return;
  }

  //Execute proper actions if an object is blocking the sensor
  if(maximumDistance-2 > distance) {
    useJaw();
  }
}

//Wag the dinosaur's tail in both directions once
void wagTail() {
  //Move the tail to middle position
  tail.write(0);

  //Wait for the movement to complete
  delay(600);

  //Move the tail to the left
  tail.write(270);

  //Wait for the movement to complete
  delay(1200);

  //Move the tail to the right
  tail.write(0);

  //Wait for the movement to complete
  delay(1200);
}

//Turn the dinosaur's head to both directions once
void turnHead() {
  //Start by moving the head back to center position
  head.write(0);

  //Wait for the movement to complete
  delay(100);

  //Move the head to the right
  head.write(180);

  //Wait for the movement to complete
  delay(1000);

  //Move the head back to the left
  head.write(0);

  //Wait for the movement to complete
  delay(1000);
}

//Open and close the dinosaur's jaw once
void useJaw() {
  //Move the jaw to the closed position
  jaw.write(180);

  //Wait for the movement to complete
  delay(300);

  //Open the jaw
  jaw.write(0);

  //Wait for the movement to complete
  delay(1200);

  //Close the jaw
  jaw.write(180);
}

