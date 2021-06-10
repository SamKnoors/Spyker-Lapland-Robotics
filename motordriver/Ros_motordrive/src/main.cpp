#include <Arduino.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Range.h>
#include <ros/time.h>
#include <Ultrasound.h>
#include <motorDrives.h>
#include <Rc_mode.h>
#include <setup.h>

unsigned long currentMillis;      //global timing value
unsigned long previousMillis;     //global timing value
int loopTime = 10;                //interval time for running main control loop

RCMODE rcmode;                    //class rc take-over sequence
ULTRASOUND_mngr safetySensors;    //class ultrasound sensors for back-up stop system
diffMotorControl motorCTRL;       //class for controlling bldc motors
ros::NodeHandle nh;               //class for rosserial communication

/*
**ROS callback & subscriber**
*gets values from topic /cmd_vel in m/s and put that in the vel to rpm converter
*/
void velCallback(  const geometry_msgs::Twist& vel)
{
  motorCTRL.velToRpm(vel.linear.x, vel.angular.z);
}
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel" , velCallback);     //create a subscriber for ROS cmd_vel topic

//Setup                       
void setup() { 
  //Serial.begin(115200);
  nh.initNode();                      // init ROS
  nh.subscribe(sub);                  //subscribe to rostopic
  motorCTRL.initMotors();             //init bldc motor comunication
  safetySensors.initSafetySensors();  //init ultrasoundsensors
  initRC();                           //init rc take-over sequence
  delay(2000);
  Serial.println("Setup done");
}

//main loop
void loop() {
  bool driveMode = rcmode.switchDrivemode();        //check if the rc control mode is activated
  nh.spinOnce();                                    //make sure ros is running
  if(!nh.connected() && !driveMode){                //when connection is lost stop the motors
    motorCTRL.velToRpm(0, 0);
    motorCTRL.driveMotors();
  }
  currentMillis = millis();
  if (currentMillis - previousMillis >= loopTime) {           // start timed loop for everything else
    previousMillis = currentMillis;
    if(driveMode == false){                                   //when automatic mode check distance ultrasound sensors
      safetySensors.readVals();
      if(safetySensors.getSensorVal(1) < maxSafetyDistance && safetySensors.getSensorVal(1) !=0){  //when one of the three sensors detect something in range of the safety distance stop the motors
      motorCTRL.velToRpm(0, 0);
      }
      if(safetySensors.getSensorVal(2) < maxSafetyDistance && safetySensors.getSensorVal(2) !=0){  //when one of the three sensors detect something in range of the safety distance stop the motors
      motorCTRL.velToRpm(0, 0);
      }
      if(safetySensors.getSensorVal(0) < maxSafetyDistance && safetySensors.getSensorVal(0) !=0){  //when one of the three sensors detect something in range of the safety distance stop the motors
      motorCTRL.velToRpm(0, 0);
      }
    }else{                                                              //when on manual mode, convert rc values to rpm command
      motorCTRL.velToRpm(rcmode.getLinearVel(), rcmode.getAngularVel());
      pinMode(2, OUTPUT);
      digitalWrite(2, HIGH);
    }
      motorCTRL.driveMotors();
  } 
} // end of main loop