#include <motorDrives.h>

//  setup communication lines for motor controll
void diffMotorControl::initMotors(){
    #ifdef Odrive
    odrive_serial.begin(115200);
    motorcalibrate(1);
    #else
    Serial1.begin(115200, SERIAL_8N1, 5, 18);
    Serial2.begin(115200);
    vesc1.setSerialPort(&Serial1);
    vesc2.setSerialPort(&Serial2);
    #endif
}
// work out the two values for differential drive of each wheel
void diffMotorControl::velToRpm(float demandX, float demandZ){
    speedMotorRight = (demandX - demandZ);
    speedMotorLeft = ((demandX + demandZ)*-1);
    #ifdef Odrive
    speedMotorLeft = speedMotorLeft/(2*3.1415*0.05);      //from velocity in m/s to rps
    speedMotorRight = speedMotorRight/(2*3.1415*0.05);      //from velocity in m/s to rps
    #else
    speedMotorLeft = constrain(1250*speedMotorLeft*7,-8750*maxSpeed,8750*maxSpeed);  //from velocity in m/s to rpm
    speedMotorRight = constrain(1250*speedMotorRight*7,-8750*maxSpeed,8750*maxSpeed);  //from velocity in m/s to rpm
    #endif
}
//  send speed command to motors
void diffMotorControl::driveMotors(){
    #ifdef Odrive
    odrive.SetVelocity(0, speedMotorLeft);
    odrive.SetVelocity(1, speedMotorRight);
    #else
    vesc1.setRPM((int)speedMotorLeft);
    vesc2.setRPM((int)speedMotorRight);
    #endif
    #ifdef debugMode
    /*Serial.print((int)speedMotorLeft);
    Serial.print("\t");
    Serial.println((int)speedMotorRight);*/
    #endif
}
// special calibration sequence for Odrive motordriver
#ifdef Odrive
void diffMotorControl::motorCalibrate(bool motor){
    int requested_state;
  for (int axis = 0; axis < 2; ++axis) {
    odrive_serial << "w axis" << axis << ".controller.config.vel_limit " << 30.0f << '\n';
    odrive_serial << "w axis" << axis << ".motor.config.current_lim " << 40.0f << '\n';
    //odrive_serial << "w axis" << axis << ".controller.config.input_mode = INPUT_MODE_VEL_RAM " <<'\n';
    // This ends up writing something like "w axis0.motor.config.current_lim 10.0\n"
  }
  requested_state = ODriveArduino::AXIS_STATE_MOTOR_CALIBRATION;
  if(!odrive.run_state(motor, requested_state, true)) return;
  requested_state = ODriveArduino::AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
  if(!odrive.run_state(motor, requested_state, true, 25.0f)) return;
  requested_state = ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL;
  if(!odrive.run_state(motor, requested_state, false /*don't wait*/)) return;
}
#endif