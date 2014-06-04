#include <Servo.h>
Servo servo, ESC;

#define AVOID_ADJUST_MAX 50

#define LEFT_IR_PIN A1
#define RIGHT_IR_PIN A0

#define STEERING_PIN 5
#define DRIVE_PIN 4

#define AVOID_THRESH 25

void setup() {
  servo.attach(STEERING_PIN);
  ESC.attach(DRIVE_PIN);
  ESC_Arming_Sequence();
  ESC.write(103);
}

void loop()
{
  double avoidLEFT = 0;
  double avoidRIGHT = 0;
  
  double leftIR =  analogRead(LEFT_IR_PIN);
  double rightIR = analogRead(RIGHT_IR_PIN);
  
  if(leftIR > AVOID_THRESH)
    avoidLEFT = map(leftIR, AVOID_THRESH, 575, 0, AVOID_ADJUST_MAX);
  if(rightIR > AVOID_THRESH)
    avoidRIGHT = map(rightIR, AVOID_THRESH, 575, 0, AVOID_ADJUST_MAX);
//  if(leftIR < AVOID_THRESH)
//    avoidLEFT = map(leftIR, AVOID_THRESH, 575, 0, AVOID_ADJUST_MAX);
//  if(rightIR < 10AVOID_THRESH0)
//    avoidRIGHT = map(rightIR, AVOID_THRESH, 575, 0, AVOID_ADJUST_MAX);
  
  servo.write(90-avoidRIGHT+avoidLEFT);
  delay(20);
}

void ESC_Arming_Sequence(){
  delay(1000);
  pinMode(DRIVE_PIN,OUTPUT);
  digitalWrite(DRIVE_PIN,HIGH);
  delay(12);
  digitalWrite(DRIVE_PIN,LOW);
  delay(8.5);
}
