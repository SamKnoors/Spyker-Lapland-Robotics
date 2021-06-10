#include "Arduino.h"
#include <setup.h>

class ULTRASOUND{
private:
    int pingPin = 0;
    long duration;
public:
    long microsecondsToCentimeters(long microseconds);
    int getRange();
    void setSensorPin(int pin)      {pingPin = pin;};
};

class ULTRASOUND_mngr{
private:
    ULTRASOUND leftRangeSensor;
    ULTRASOUND centerRangeSensor;
    ULTRASOUND rightRangeSensor;
public:
    int distSensorVals[2];
    void initSafetySensors();
    void readVals();
    int getSensorVal(uint8_t sensorNum)       {return distSensorVals[sensorNum];};
};