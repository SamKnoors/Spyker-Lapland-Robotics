#include <Arduino.h>
#include <setup.h>

#ifdef Odrive
#include "ODriveArduino.h"
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }
#else
#include <VescUart.h>
#endif

class diffMotorControl{
    private:
        float speedMotorLeft = 0;
        float speedMotorRight = 0;
        #ifdef Odrive
        HardwareSerial& odrive_serial = Serial2;
        ODriveArduino odrive(odrive_serial);
        #else
        VescUart vesc1;
        VescUart vesc2;
        #endif
    public:
        void initMotors();
        void motorCalibrate(bool motor);
        void velToRpm(float demandX, float demandZ);
        void driveMotors();
};
