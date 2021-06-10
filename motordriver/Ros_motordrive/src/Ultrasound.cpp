#include "Arduino.h"
#include "Ultrasound.h"

//distance calculation with the speed of sound.
long ULTRASOUND::microsecondsToCentimeters(long microseconds){
    return microseconds / 29.1 / 2;
}

// establish variables for duration of the ping,
// and the distance result centimeters:
int ULTRASOUND::getRange(){  
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  
  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  
  // convert the time into a distance
  return microsecondsToCentimeters(duration);
  //Serial.println(microsecondsToCentimeters(duration));
}

//configure io pins
void ULTRASOUND_mngr::initSafetySensors(){
  leftRangeSensor.setSensorPin(usoundLeftPin);
  centerRangeSensor.setSensorPin(usoundCenterPin);
  rightRangeSensor.setSensorPin(usoundRightPin);
}

//read distances from ultrasound sensors
void ULTRASOUND_mngr::readVals(){
  distSensorVals[0] = leftRangeSensor.getRange();
  distSensorVals[1] = centerRangeSensor.getRange();
  distSensorVals[2] = rightRangeSensor.getRange();
};