#include "Arduino.h"
#include <Servo.h>

/*
    Rock Canyon TSA Animatronics
    First made January 11th 2017
    Last appended to January 27th 2017
*/

//Constant motor positions
unsigned const int SERVO_LEFT_POINT = 0, SERVO_MID_POINT = 90, SERVO_RIGHT_POINT = 180;

//Servo pins
unsigned const int motor_head1 = 5, motor_head2 = 6, motor_tail = 8;

//Servo objects <Servo.h>
Servo head1, head2, tail;

//Sets the mode to production by default
bool demoEnabled = false;

//The loop will only run if this is true
bool running = false;

/*
    Servo pointers:
    head1 -> x-axis (left and right)
    head2 -> z-axis (up and down)
    tail -> x-axis (left and right)
*/

/*
  Prints the avalible commands at setup
*/
void printHelp() {
  Serial.println("--Avalible commands--");
  Serial.println("p -> Set mode to production");
  Serial.println("d -> Set mode to demo");
  Serial.println("r -> Reset dinosaur, move motors to default positions");
  Serial.println("t -> Set running back to true");
}

//The setup method, the first one that runs
void setup() {

  //Create the serial connections
  Serial.begin(9600);
  printHelp();
  //Ensure valid data by flushing the buffer
  Serial.flush();

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

  //Start the loop
  running = true;
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
  The demo mode, it runs a continous loop of every actions to show what
  the dinosaur can do instead of it being random
*/
void demo() {
  //Turn head then delay 1.5 seconds
  action_HeadTurn();
  delay(1500);

  //Wag tail then delay 1.5 seconds
  action_TailWag(random(3, 6));
  delay(1500);

  //Shake head then delay 1.5 seconds
  action_HeadShake(random(2, 4));
  delay(1500);

  //Wag tail then shake head and then delay 1.5 seconds
  action_HeadTurn();
  action_TailWag(random(4, 7));
  delay(1500);
}

/*
  The production mode, picks ONE random action with a random delay period between the method release
*/
void production() {
  int rnd = random(0, 3);

  //Choose an action from the random number
  switch(rnd) {
    case 0: action_HeadTurn();
    case 1: action_TailWag(random(3, 5));
    case 2: action_HeadShake(random(2, 4));
  }

  //Random delay period
  delay(random(2000, 6500));
}

/*
  Constantly checks for new input from the serial console and updates modes or
  forces actions based on those commands, otherwise it continues per usual
*/
void loop() {
  //Only run the loop if variable running is true
  if(running) {

    //Only read if there is data
    if(Serial.available() > 0) {
      char cmd = Serial.read();
      Serial.flush();

      //Run proper methods from the commands
      switch(cmd) {
        //p or P is production mode enabled
        case 'p':
        case 'P':
          demoEnabled = false;
          Serial.println("Set mode to production!");
          break;

        //d or D is demo mode enabled
        case 'd':
        case 'D':
          demoEnabled = true;
          Serial.println("Set mode to demo!");
          break;

        //r or R is reset
        case 'r':
        case 'R':
          Serial.println("Resetting dinosaur to defaults...");
          head1.write(SERVO_MID_POINT);
          head2.write(SERVO_MID_POINT);
          tail.write(SERVO_MID_POINT);
          running = false;
          break;

          //t or T sets running to true
          case 't':
          case 'T':
            running = true;
            Serial.println("Set running to true!");
      }
    }

    //Run proper loop per the mode
    if(demoEnabled) {
      //Run demo mode
      demo();
    } else if(!demoEnabled) {
      //Run production mode
      production();
    }
  }
}
