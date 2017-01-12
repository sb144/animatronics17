#include "Arduino.h"
#include <Servo.h>

/*
    Rock Canyon TSA Animatronics
    First made January 11th 2017
    Last appended to January 11th 2017
*/

//Constant motor positions
unsigned const int SERVO_LEFT_POINT = 0, SERVO_MID_POINT = 90, SERVO_RIGHT_POINT = 180;

//Servo pins
unsigned const int motor_head1 = 5, motor_head2 = 6, motor_tail = 8;

//Servo objects <Servo.h>
Servo head1, head2, tail;

/*
    head1 -> x-axis (left and right)
    head2 -> z-axis (up and down)
    tail -> x-axis (left and right)
*/

//The setup method, the first one that runs
void setup() {
  //Set the pins for all of the servos
  head1.attach(motor_head1);
  head2.attach(motor_head2);
  tail.attach(motor_tail);

  //Set default pulse times (IN MICROSECONDS not milliseconds)
  head1.writeMicroseconds(1500); //1.5 ms
  head2.writeMicroseconds(1500);
  tail.writeMicroseconds(1500);

  //Set all servos to thier default positions
  head1.write(SERVO_MID_POINT);
  head2.write(SERVO_MID_POINT);
  tail.write(SERVO_MID_POINT);

  //Seed the random generator
  randomSeed(42);
}


/*
    Actions
*/

//Make the head turn from left to right (as if it's looking around)
void action_HeadTurn() {
  head1.write(SERVO_MID_POINT - 45);
  delay(900);
  head1.write(SERVO_MID_POINT + 45);
  delay(900);
  head1.write(SERVO_MID_POINT);
}

//Wag the tail (iterations) times
void action_TailWag(int iterations) {
  for(int i = 0; i < iterations; i++) {
    head1.write(SERVO_MID_POINT - 45);
    delay(500);
    head1.write(SERVO_MID_POINT + 45);
    delay(500);
  }
}

//Shake the dinosaur's head up and down (like it's saying yes)
void action_HeadShake(int iterations) {
  for(int j = 0; j < iterations; j++) {
    head2.write(SERVO_MID_POINT + 45);
    delay(500);
    head2.write(SERVO_MID_POINT - 45);
    delay(500);
  }
}

/*
  The second method called, loops continously until the Arduino is halted
  Contains the logic that moves the parts of the dinosaur
  The loop will take anywhere between 35 <-> 60 seconds
*/
void loop() {

  //Turn head then delay between 3-9 seconds
  action_HeadTurn();
  delay(random(3000, 9000));

  //Wag tail then delay 5-11 seconds
  action_TailWag(random(3, 6));
  delay(random(5000, 11000));

  //Shake head then delay 6-10 seconds
  action_HeadShake(random(2, 4));
  delay(random(6000, 10000));

  //Wag tail then shake head and then delay 9-15 seconds
  action_HeadTurn();
  action_TailWag(random(4, 7));
  delay(random(9000, 15000));

}
