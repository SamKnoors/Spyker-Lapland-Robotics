#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/Imu.h>
#include <zed_interfaces/Objects.h>
#include <zed_interfaces/ObjectStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>

#include <string>
 
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

void objectListCallback(const zed_interfaces::Objects::ConstPtr& msg)
{
  ROS_INFO("***** New object list *****");
  for (int i = 0; i < msg->objects.size(); i++)
    {
    if (msg->objects[i].label_id == -1)
        continue;


    ROS_INFO("Speed decreased");
   //system("rosparam set /move_base/TrajectoryPlannerROS/max_vel_x 2.0");
    system("rosrun dynamic_reconfigure dynparam set /move_base/TrajectoryPlannerROS \"{'max_vel_x': 2}\" ");
    ROS_INFO_STREAM( msg->objects[i].label
                    << " ["
                    << msg->objects[i].label_id
                    << "] - Pos. ["
                    << msg->objects[i].position.x << ","
                    << msg->objects[i].position.y << ","
                    << msg->objects[i].position.z << "] [m]"
                    << "- Conf. "
                    << msg->objects[i].confidence
                    << " - Tracking state: "
                    << static_cast<int>(msg->objects[i].tracking_state) );
    }
  if(msg->objects.size() == 0)
  {
    ROS_INFO("Speed normal");
    //system("rosparam set /move_base/TrajectoryPlannerROS/max_vel_x 4.0");
    system("rosrun dynamic_reconfigure dynparam set /move_base/TrajectoryPlannerROS \"{'max_vel_x': 1}\" ");
  }
}
void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{

}

 int main(int argc, char* argv[])
 {
    ros::init(argc, argv, "zed_sensor_node");
    
    ros::NodeHandle n;
  
    ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time = ros::Time::now();
    
    ros::Rate rate(0.5);
    ROS_INFO("node started");
    while(ros::ok())
    {

    last_time = current_time;
        //sub to pose, otherwise objects topic won't publish
        ros::Subscriber subPose = n.subscribe("/zed2/zed_node/pose", 10, poseCallback);
        ros::Subscriber subobj = n.subscribe("/zed2/zed_node/obj_det/objects", 1, objectListCallback);
        ros::Subscriber subImu = n.subscribe("/zed2/zed_node/imu/data_raw", 1, imuCallback);
        ros::Subscriber subDepth = n.subscribe("/zed2/zed_node/depth/depth_registered", 1, depthCallback);
        rate.sleep();
        ros::spinOnce();
        current_time = ros::Time::now();

	//ros::spin();
    }
    return 0;
 }

 
