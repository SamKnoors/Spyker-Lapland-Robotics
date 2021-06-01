# Spyker-Lapland-Robotics

Original the Spyker is a radio controlled snow blower made out 3d printed parts. In this repository you will find the code for making from this machine a full autonomous robot using a stereolabs zed 2 depth cam, a slamtech RPLiDAR A3 and multiple grove ultrasound sensors. The Spyker makes use of two BLDC motors in combination with a single board motor controller from Flipsky named the FSESC 6.6. This esc is controlled by using UART communication coming from a ESP32. Also, the ultrasound sensors are connected to the microcontroller. The brain of this robot is a Jetson TX2 that runs a robot operating system (ROS) to connect every component together. This project makes use of SLAM for localization and mapping and does not need odometry feedback from the motors or IMU. A step-by-step guide can be found in the wiki.  

![Spyker](https://github.com/SamKnoors/Spyker-Lapland-Robotics/blob/main/20210528_124252.jpg)
