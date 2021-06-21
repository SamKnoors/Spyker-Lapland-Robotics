cmake_minimum_required(VERSION 3.0.2)
project(zed_sensors)

# if CMAKE_BUILD_TYPE is not specified, take 'Release' as default
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT CMAKE_BUILD_TYPE)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp sensor_msgs zed_interfaces tf)

## Declare a catkin package
catkin_package()
include_directories(include ${catkin_INCLUDE_DIRS})

## Build 
add_executable(zed_sensors src/camera_sensors.cpp)
target_link_libraries(zed_sensors ${catkin_LIBRARIES})
add_dependencies(zed_obj_det_sub ${catkin_EXPORTED_TARGETS})
