#include "Arduino.h"
#include <setup.h>

#define signalPin1 22
#define signalPin2 23



void initRC();
void pulsTimeSignal1();
void pulsTimeSignal2();

class RCMODE{
    private:
    unsigned long timerStartPoint;
    unsigned long timeNow;
    bool flag1 = false;
    int flag2 = 0;
    float demandX = 0;
    float demandZ = 0;   
    public:
    bool switchDrivemode();
    void diffDrive();
    float getLinearVel()     {diffDrive(); return demandX;};
    float getAngularVel()     {diffDrive(); return demandZ;};
};
