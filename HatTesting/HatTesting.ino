#include "SR04.h" //To read from the sensors

//GLOBAL VARIABLES FOR MOTOR CONTROLS
int pwm_a = 3;  //PWM control for motor outputs 1 and 2 
int pwm_b = 9;  //PWM control for motor outputs 3 and 4 
int dir_a = 2;  //direction control for motor outputs 1 and 2 
int dir_b = 8;  //direction control for motor outputs 3 and 4

//DEFINITIONS FOR ULTRASONIC SENSORS
#define SENSOR1ECHO 4
#define SENSOR1TRIG 5
#define SENSOR2ECHO 6
#define SENSOR2TRIG 7

//DYNAMIC GLOBAL VARIABLES
SR04 sensor1 = SR04(SENSOR1ECHO, SENSOR1TRIG); //The class object for sensor 1
SR04 sensor2 = SR04(SENSOR2ECHO, SENSOR2TRIG); //The class object for sensor 2
long distSensor1;
long distSensor2;
float quarterSecAverageDistLeft;
float quarterSecAverageDistRight;

//Prototypes


void setup()
{
  Serial.begin(9600);
  delay(1000);
  
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
  
  analogWrite(pwm_a, 100);  //set both motors to run at (100/255 = 39)% duty cycle (slow)
  analogWrite(pwm_b, 100);
  
}

void loop()
{
  /*Directional Testing
  Serial.println("Forward");
  analogWrite(pwm_a, 0); //Takes a quarter second to stop spinning the motors
  analogWrite(pwm_b, 0); 
  delay(500);
  forward(5);

  Serial.println("Back");
  analogWrite(pwm_a, 0); //Takes a quarter second to stop spinning the motors
  analogWrite(pwm_b, 0); 
  delay(500);
  back(5);

  Serial.println("Right");
  analogWrite(pwm_a, 0); //Takes a quarter second to stop spinning the motors
  analogWrite(pwm_b, 0); 
  delay(500);
  right(90);

  Serial.println("Left");
  analogWrite(pwm_a, 0); //Takes a quarter second to stop spinning the motors
  analogWrite(pwm_b, 0); 
  delay(500);
  left(90);

  delay(5000); //Waits 5 seconds and then repeats */
  
  //Getting the two second distance average
  quarterSecAverageDistLeft = 0;
  quarterSecAverageDistRight = 0;
  int count = 0;
  int notTimeUp = 1;
  int timer = 0;
  
  while(notTimeUp){
    quarterSecAverageDistLeft += distSensor2;
    distSensor2 = sensor2.Distance();
    quarterSecAverageDistRight += distSensor1;
    timer += 25;
    count += 1;
    delay(25);
    if(timer == 250){
      notTimeUp = 0;
    }
  }
  quarterSecAverageDistLeft = quarterSecAverageDistLeft / count;
  quarterSecAverageDistRight = quarterSecAverageDistRight /count;
  
  Serial.print("2 Second Average Left: ");
  Serial.println(quarterSecAverageDistLeft);
  Serial.print("2 Second Average Right: ");
  Serial.println(quarterSecAverageDistRight);
  //Moving the motors
  if(quarterSecAverageDistLeft < 50 && quarterSecAverageDistLeft > 5){
    analogWrite(pwm_a, 0); //Takes a quarter second to stop spinning the motors
    analogWrite(pwm_b, 0); 
    delay(250);
    right(90);
  }
  else if(quarterSecAverageDistRight < 50 && quarterSecAverageDistRight > 5) {
    analogWrite(pwm_a, 0); //Takes a quarter second to stop spinning the motors
    analogWrite(pwm_b, 0); 
    delay(250);
    left(90);
  }
  else{
    analogWrite(pwm_a, 0); //Takes a quarter second to stop spinning the motors
    analogWrite(pwm_b, 0); 
    delay(250);
    forward(5);
  }
}

void left(float degree){

  float degreeToMS = degree * 50;
  /*analogWrite(pwm_a, 0);  //Takes a quarter second to stop spinning the motors
  analogWrite(pwm_b, 0);
  delay(250);*/
  
  digitalWrite(dir_a, LOW); //Sets direction to forward 
  digitalWrite(dir_b, LOW);  
  
  analogWrite(pwm_a, 70); //Moves forward for 2.5 seconds
  analogWrite(pwm_b, 70);
  
  delay(degreeToMS);
}

void right(float degree){
  
  float degreeToMS = degree * 50;
  /*analogWrite(pwm_a, 0);  //Takes a quarter second to stop spinning the motors
  analogWrite(pwm_b, 0);
  delay(250);*/
  
  digitalWrite(dir_a, HIGH); //Sets direction to backward
  digitalWrite(dir_b, HIGH);  
  
  analogWrite(pwm_a, 70); //Moves backward for one second
  analogWrite(pwm_b, 70);
  
  delay(degreeToMS); //Wait half a second
}

void back(float seconds){

  float microseconds = seconds * 1000;
  
  /*analogWrite(pwm_a, 0); //Takes a quarter second to stop spinning the motors
  analogWrite(pwm_b, 0); 
  delay(250);*/

  digitalWrite(dir_a, HIGH); //Sets left motor forward
  digitalWrite(dir_b, LOW); //Sets right motor backward

  analogWrite(pwm_a, 70); //Left motor forward at 100 magnitude
  analogWrite(pwm_b, 70); //Right motor forward at 100 magnitude

  delay(microseconds); //Motors are active for half of a second
}

void forward(float seconds){

  float microseconds = seconds * 1000;
  /*analogWrite(pwm_a, 0); //Takes a quarter second to stop spinning the motors
  analogWrite(pwm_b, 0); 
  delay(250);*/

  digitalWrite(dir_a, LOW); //Sets left motor backward
  digitalWrite(dir_b, HIGH); //Sets right motor forward

  analogWrite(pwm_a, 70); //Left motor forward at 100 magnitude
  analogWrite(pwm_b, 70); //Right motor forward at 100 magnitude

  delay(microseconds); //Motors are active for half of a second
}
