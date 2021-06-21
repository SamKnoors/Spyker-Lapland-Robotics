//Route planning code
#include <ros/ros.h>
#include <nav_msgs/GetPlan.h>
#include <geometry_msgs/PoseStamped.h>
#include <string>
#include <boost/foreach.hpp>
#define forEach BOOST_FOREACH
void fillPathRequest(nav_msgs::GetPlan::Request &request)
{
request.start.header.frame_id ="map";
request.start.pose.position.x = 12.378;//x coordinate of the initial position
request.start.pose.position.y = 28.638;//y coordinate of the initial position
request.start.pose.orientation.w = 1.0;//direction
request.goal.header.frame_id = "map";
request.goal.pose.position.x = 18.792;//End point coordinates
request.goal.pose.position.y = 29.544;
request.goal.pose.orientation.w = 1.0;
request.tolerance = 0.5;//If the goal cannot be reached, the most recent constraint
}
//Callback for route planning results
void callPlanningService (ros :: ServiceClient & serviceClient, nav_msgs :: GetPlan & srv)
{
//Perform the actual path planner call
//Execute the actual path planner
if (serviceClient .call(srv)) {
//srv.response.plan.poses is the container to save the result, traverse and take out
if (! srv .response.plan.poses.empty()) {
forEach(const geometry_msgs::PoseStamped &p, srv.response.plan.poses) {
ROS_INFO("x = %f, y = %f", p.pose.position.x, p.pose.position.y);
}
}
else {
ROS_WARN("Got empty plan");
}
}
else {
ROS_ERROR("Failed to call service %s - is the robot moving?",
serviceClient.getService().c_str());
}
}

int main(int argc, char** argv)
{
ros::init(argc, argv, "make_plan_node");
ros::NodeHandle nh;
//Init service query for make plan
//Initialize path planning service, service name is "move_base_node/make_plan"
std::string service_name = "move_base_node/make_plan";
//Wait for the service to be idle. If the service is already running, it will wait until the end of the operation.
while (! ros :: service :: waitForService (service_name, ros :: Duration ( 3.0))) {
ROS_INFO("Waiting for service move_base/make_plan to become available");
}
/* Initialize the client, (nav_msgs/GetPlan) Allows an external user to ask for a plan to a given pose from move_base without causing move_base to execute that plan. Allows the user to request a plan from move_base and does not cause move_base to execute this plan */ 
ros::ServiceClient serviceClient = nh.serviceClient<nav_msgs::GetPlan>(service_name, true);
if (!serviceClient) {
ROS_FATAL("Could not initialize get plan service from %s",
serviceClient.getService().c_str());
return -1;
}
nav_msgs::GetPlan srv;
//Request service: plan route
fillPathRequest (srv .request);
if (!serviceClient) {
ROS_FATAL("Persistent service connection to %s failed",
serviceClient.getService().c_str());
return -1;
}
ROS_INFO("conntect to %s",serviceClient.getService().c_str());
callPlanningService(serviceClient, srv);
} 
