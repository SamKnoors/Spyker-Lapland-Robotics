#include "Rc_mode.h"

unsigned long startTime1;
unsigned long startTime2;
int pulsLength1;
int pulsLength2;

//init rc input pins
void initRC(){
    pinMode(signalPin1, INPUT);
    pinMode(signalPin2, INPUT);
    attachInterrupt(signalPin1, pulsTimeSignal1, CHANGE);
    attachInterrupt(signalPin2, pulsTimeSignal2, CHANGE);
}

//measure puls duration for ppm translation for channel 1 
void pulsTimeSignal1(){
    if (digitalRead(signalPin1) == HIGH) {
      startTime1 = micros();
    }
    else {
      pulsLength1 = micros() - startTime1;
    }
}

//measure puls duration for ppm translation for channel 1
void pulsTimeSignal2(){
    if (digitalRead(signalPin2) == HIGH) {
      startTime2 = micros();
    }
    else {
      pulsLength2 = micros() - startTime2;
      //Serial.println(pulsLength2);
    }
}

//rc take-over sequence.
//tap 2 time two times max bw throttle whithin 500ms to activate manual mode 
bool RCMODE::switchDrivemode(){
  if(pulsLength2 > 1900 && flag1 == false && flag2 == 0){
    timerStartPoint = millis();
    flag2 = 1;
  }
  while(timeNow < timerStartPoint + 500 && flag1 == false){
    timeNow = millis();
    if(flag2 == 1 && pulsLength2 > 1400 && pulsLength2 < 1600){
      flag2 = 2;
    }
    if(flag2 == 2 &&(pulsLength2 < 1100 || pulsLength2 > 1900)){
      flag2 = 3;
    }
    if(flag2 == 3 && pulsLength2 > 1400 && pulsLength2 < 1600){
      flag1 = true;
      flag2 = 0;
    }
    /*if(millis() > timerStartPoint + 500){
      flag2 = 0;
    }*/
  }
  flag2 = 0;
  
  if(flag1 == true){
    #ifdef debugMode
    Serial.println("RC mode activated!");
    #endif
    if(pulsLength2 < 1400 || pulsLength2 > 1600 || pulsLength1 < 1400 || pulsLength1 > 1600){
      timerStartPoint = millis();
    }
    if(millis() > timerStartPoint +10000){
      #ifdef debugMode
      Serial.println("RC mode deactivated!");
      #endif
      flag1 = false;
      flag2 = 0;
    }
  }
  return flag1;
}

//put measured results into diff drive values
void RCMODE::diffDrive(){
  float outputX;
  float outputZ;
  if(pulsLength1 > 1440 && pulsLength1 < 1560){
    pulsLength1 = 1500;
  }
  if(pulsLength2 > 1440 && pulsLength2 < 1560){
    pulsLength2 = 1500;
  }
  outputX = map(pulsLength2,1000,2000,-100*maxSpeed, 100*maxSpeed);
  outputZ = map(pulsLength1,1000,2000,-100*maxSpeed, 100*maxSpeed);
  demandX = outputX/100;
  demandZ = outputZ/100;
}

