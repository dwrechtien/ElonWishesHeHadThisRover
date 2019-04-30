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
float quarterSecAverageDist1;
float quarterSecAverageDist2;

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
  //Getting the two second distance average
  quarterSecAverageDist1 = 0;
  quarterSecAverageDist2 = 0;
  int count = 0;
  int notTimeUp = 1;
  int timer = 0;
  
  while(notTimeUp){
    distSensor1 = sensor1.Distance();
    if(distSensor1 > 1000){
      distSensor1 = 0;
    }
    quarterSecAverageDist1 += distSensor1;
    distSensor2 = sensor2.Distance();
    if(distSensor2 > 1000){
      distSensor2 = 0;
    }
    quarterSecAverageDist2 += distSensor2;
    timer += 50;
    count += 1;
    delay(25);
    if(timer == 250){
      notTimeUp = 0;
    }
  }
  quarterSecAverageDist1 = quarterSecAverageDist1 / count;
  quarterSecAverageDist2 = quarterSecAverageDist2 /count;
  
  Serial.print("How far object 1 is 2 sec average in cm: ");
  Serial.println(quarterSecAverageDist1);
  Serial.print("How far object 1 is 2 sec average in cm: ");
  Serial.println(quarterSecAverageDist2);
  //Moving the motors
  if(quarterSecAverageDist1 > quarterSecAverageDist2){
     forward();
  }
  if(quarterSecAverageDist2 > quarterSecAverageDist1){
    back();
  }
}

void forward(){
  analogWrite(pwm_a, 0);  //Takes half a second to stop spinning the motors
  analogWrite(pwm_b, 0);
  
  delay(250); //Pauses for half of a second
  
  digitalWrite(dir_a, LOW); //Sets direction to forward 
  digitalWrite(dir_b, LOW);  
  
  analogWrite(pwm_a, 200); //Moves forward for 2.5 seconds
  analogWrite(pwm_b, 200);
  
  delay(1000);
}

void back(){
  analogWrite(pwm_a, 0);  //Takes half a second to stop spinning the motors
  analogWrite(pwm_b, 0);
  
  delay(250); //Pauses for half of a second
  
  digitalWrite(dir_a, HIGH); //Sets direction to backward
  digitalWrite(dir_b, HIGH);  
  
  analogWrite(pwm_a, 200); //Moves backward for one second
  analogWrite(pwm_b, 200);
  
  delay(1000);
}
