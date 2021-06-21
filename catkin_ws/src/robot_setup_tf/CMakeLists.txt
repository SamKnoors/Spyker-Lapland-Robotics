cmake_minimum_required(VERSION 3.0.2)
project(robot_setup_tf)


find_package(catkin REQUIRED COMPONENTS geometry_msgs nav_msgs roscpp tf)

catkin_package()

include_directories(${catkin_INCLUDE_DIRS})
add_executable(tf_broadcaster src/tf_broadcaster.cpp)
add_executable(tf_listener src/tf_listener.cpp)
add_executable(laserscan src/publish_laserscan.cpp)
add_executable(pointcloud src/publish_pointcloud.cpp)

target_link_libraries(tf_broadcaster ${catkin_LIBRARIES})
target_link_libraries(tf_listener ${catkin_LIBRARIES})
target_link_libraries(laserscan ${catkin_LIBRARIES})
target_link_libraries(pointcloud ${catkin_LIBRARIES})

