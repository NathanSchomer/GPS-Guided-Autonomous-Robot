#include <TinyGPS++.h>
TinyGPSPlus gps;

#include <Servo.h>   //servo library for steering and ESC
#include <Wire.h>    //wire library for compass
Servo servo, ESC;    //ESC and steering servo initialization

int target = 3;                                         //Counter tracks next waypoints
static const double waypoint[] = {39.9570, -75.1885,    //Waypoints
                                  39.9369, -75.1886,
                                  39.9568, -75.1882,
                                  39.9572, -75.1889};
                                 
#define STEERING_PIN 5
#define DRIVE_PIN 4

//targret- current

#define STEER_SCALING 1
#define STEER_CENTERPOINT 90
#define STEER_CENTER_ADJUST 5 //  +/-5

#define STOP 90

int HMC6352Address = 0x42;
int i, headingValue, slaveAddress;
double currHeading;
double newHeading;
byte headingData[2];
 
void setup() {
  Serial1.begin(4800);
  slaveAddress = HMC6352Address >> 1; 
  Serial.begin(9600); 
  Wire.begin();
  
  servo.attach(STEERING_PIN);
  ESC.attach(DRIVE_PIN);
  ESC_Arming_Sequence();
  ESC.write(106);
} 

void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.

  while (Serial1.available() > 0)
    gps.encode(Serial1.read());
    
    
    steerUpdate(0);
    
    distanceCheck();
       
       
//  if (millis() > 5000 && gps.charsProcessed() < 10)
//  {
//    Serial.println(F("No GPS detected: check wiring."));
//    while(true);
//  }
      


} 

void steerUpdate(int input){
  double bearing;
    bearing = TinyGPSPlus::courseTo(gps.location.lat(),gps.location.lng(),  39.9574, -75.1888);
  //bearing = 180;
  double dTheta = getHeading();
  double nTheta = adjustHeading(bearing,dTheta);

  if(input == 0){
    
    if(nTheta > 0 && nTheta < 180){
      servo.write((STEER_CENTERPOINT)-STEER_SCALING*nTheta);
      delay(15);
    }
    else if(nTheta > 180 && nTheta < 360){
      servo.write((360+STEER_CENTERPOINT)-STEER_SCALING*nTheta); //90-(1/5)*input[1]
      delay(15);
    }
    
  }
  //else if(input == 1){/*servo.write(input);*/}
}


double adjustHeading(double bearing, double  dTheta){
  if(dTheta >= bearing){
      newHeading = dTheta - bearing;
      Serial.println(newHeading);
      return newHeading;

    }
    if(bearing > dTheta){
      newHeading = (360-(bearing-dTheta));
      Serial.println(newHeading);
      return newHeading;

    }  
}


double getHeading(){
    /*Wire.beginTransmission(slaveAddress);
    Wire.write("A");               //Get Data
    Wire.endTransmission();
    delay(10);                   // 70us delay
    Wire.requestFrom(slaveAddress, 2);        // Request the 2 byte heading
    i = 0;
    while(Wire.available() && i < 2)
    {
      headingData[i] = Wire.read();
      i++;
    }
    
    currHeading = (headingData[0]*256 + headingData[1])/10;  // Put the MSB and LSB together and divide by 10
  //Serial.println(currHeading);
    return currHeading;*/
    return gps.course.deg();
       
}


void ESC_Arming_Sequence(){
  delay(1000);
  pinMode(DRIVE_PIN,OUTPUT);
  digitalWrite(DRIVE_PIN,HIGH);
  delay(12);
  digitalWrite(DRIVE_PIN,LOW);
  delay(8.5);
}

void distanceCheck(){
  double distance = TinyGPSPlus::distanceBetween(gps.location.lat(),gps.location.lng(),waypoint[target], waypoint[target+1])/1000;
  if(distance <= 3){
    ESC.write(STOP);
  }
  //if(target+1 == sizeof(waypoint)){
   // ESC.write(STOP);
  //} 
}

