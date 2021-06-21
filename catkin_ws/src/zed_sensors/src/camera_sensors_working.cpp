#include <ros/ros.h>
//#include <rados/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/Imu.h>
 

void imuCallback(const sensor_msgs::Imu::ConstPtr& msg)
{
  ROS_INFO("Accel: %.3f,%.3f,%.3f [m/s^2] - Ang. vel: %.3f,%.3f,%.3f [deg/sec]",
           msg->linear_acceleration.x, msg->linear_acceleration.y, msg->linear_acceleration.z, msg->angular_velocity.x,
           msg->angular_velocity.y, msg->angular_velocity.z);
}

void depthCallback(const sensor_msgs::Image::ConstPtr& msg)
{
  // Get a pointer to the depth values casting the data
  // pointer to floating point
  float* depths = (float*)(&msg->data[0]);

  // Image coordinates of the center pixel
  int u = msg->width / 2;
  int v = msg->height / 2;

  // Linear index of the center pixel
  int centerIdx = u + msg->width * v;

  // Output the measure
  ROS_INFO("Center distance : %g m", depths[centerIdx]);
}


 int main(int argc, char* argv[])
 {
    ros::init(argc, argv, "zed_sensor_node");
    
    ros::NodeHandle n;
    ros::Rate rate(1);
    ROS_INFO("node started");
    while(ros::ok())
    {
        ros::Subscriber subImu = n.subscribe("/zed2/zed_node/imu/data_raw", 1, imuCallback);
        ros::Subscriber subDepth = n.subscribe("/zed2/zed_node/depth/depth_registered", 1, depthCallback);

	rate.sleep();
        ros::spinOnce();
    }
    return 0;
 }

 
