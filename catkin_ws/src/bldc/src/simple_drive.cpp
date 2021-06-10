#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
#include <sstream>


ros::Time startTime;
ros::Time stopTime;

uint64_t latency;

bool flag = true;
uint8_t count = 0;


void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
	stopTime = ros::Time::now();
	ROS_INFO("I heard: [%s]", msg->data.c_str());
	flag = true;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "replier");

  ros::NodeHandle n;


  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("vesc", 100);
  
  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);

  ros::Rate loop_rate(1);
  
  std_msgs::String msg;

  char string[] = "STOP";
  msg.data = string;
  /*for(int i=0; i<3; i++){
    chatter_pub.publish(msg);
  }*/
  

  while (ros::ok())
  {
    if(flag == true){
      latency = stopTime.toNSec()-startTime.toNSec();
      ROS_INFO(" %zd ns",latency);
      
      flag = false;
    }
    chatter_pub.publish(msg);
    startTime = ros::Time::now();
    ros::spinOnce();
    loop_rate.sleep();
  }


  return 0;
}
