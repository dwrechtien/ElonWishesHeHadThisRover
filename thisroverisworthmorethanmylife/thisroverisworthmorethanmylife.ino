/*
  yo what's up everybody this is Derek welcome to my code it's pretty hype let me show you around
*/
//this library contains all the ultrasonic stuff we're using
#include<NewPing.h>

//these are the pins for the ultrasonic sensors, subject to change
#define TRIGGER_PIN1 12
#define ECHO_PIN1 11
#define TRIGGER_PIN2 10
#define ECHO_PIN2 7
//distance in inches that we don't want
#define MAX_DISTANCE 15

//these pin numbers are completely arbitrary and should be changed to fit our needs when decided
int pwm_l = 3;//PWM control for motor outputs 1 and 2
int pwm_r = 9; //PWM control for motor outputs 3 and 4
int dir_l = 2; //direction control for motor outputs 1 and 2
int dir_r = 8; //direction control for motor outputs 3 and 4

//keeps track of how far we've wandered from center
int degreesFromCenter = 0;

//I copied this from a website so if it doesn't work don't blame me
NewPing sonarl(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
NewPing sonarr(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);


void setup() {
  //Set control pins to be outputs
  pinMode(pwm_l, OUTPUT);
  pinMode(pwm_r, OUTPUT);
  pinMode(dir_l, OUTPUT);
  pinMode(dir_r, OUTPUT);
}

void loop() {
  //this will either tell the motors to go forwards or backwards
  digitalWrite(dir_l, LOW);
  digitalWrite(dir_r, LOW);

  //this powers the motors to (50/255)% power
  analogWrite(pwm_l, 50);
  analogWrite(pwm_r, 50);

  //go for 50 time
  delay(50);
  
  //stops the rover from moving indefinitely
  analogWrite(pwm_r, 0);
  analogWrite(pwm_l, 0);

  //time for the actual code that might be useful
  if(sonarl.ping_in() > 0 || sonarr.ping_in() > 0)
    objectAvoidance();

}

//there's a 50% chance this will actually turn left, who knows
//turns the rover 45ish degrees right
void right_turn()
{
  //this flips the directions of the motors to turn it
  digitalWrite(dir_r, LOW);
  digitalWrite(dir_l, HIGH);

  //turns the bitch
  analogWrite(pwm_r, 50);
  analogWrite(pwm_l, 50);
  delay(50);

  //this will stop the rover from turning
  analogWrite(pwm_r, 0);
  analogWrite(pwm_l, 0);
  
  degreesFromCenter = degreesFromCenter + 45;
}

//there's a 50% chance this will actually turn right, who knows
//turns the robot 45ish degrees right
void left_turn()
{
  //this flips the directions of the motors to turn it
  digitalWrite(dir_l, HIGH);
  digitalWrite(dir_r, LOW);

  //turns the bitch
  analogWrite(pwm_l, 50);
  analogWrite(pwm_r, 50);
  delay(50);

  //stops the rover from turning endlessly
  analogWrite(pwm_l, 0);
  analogWrite(pwm_r, 0);

  degreesFromCenter = degreesFromCenter - 45;
}

void forward()
{
  //this will either tell the motors to go forwards or backwards
  digitalWrite(dir_l, LOW);
  digitalWrite(dir_r, LOW);

  //this powers the motors to (50/255)% power
  analogWrite(pwm_l, 50);
  analogWrite(pwm_r, 50);
  
  delay(50);
  
  analogWrite(pwm_l, 0);
  analogWrite(pwm_r, 0);
}

void objectAvoidance()
{
  if(sonarl.ping() > sonarr.ping())
  {
    if(sonarr.ping() == 0)
    {
      left_turn();
      forward();
      right_turn();
    }
    else
    {
      right_turn();
      forward();
      left_turn();
    }
  }
  else
  {
    if(sonarl.ping() == 0)
    {
      right_turn();
      forward();
      left_turn();
    }
    else
    {
      left_turn();
      forward();
      right_turn();
    }
  }
}
