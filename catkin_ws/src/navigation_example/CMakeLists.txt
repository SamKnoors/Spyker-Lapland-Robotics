cmake_minimum_required(VERSION 3.0.2)
project(navigation_example)

# if CMAKE_BUILD_TYPE is not specified, take 'Release' as default
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT CMAKE_BUILD_TYPE)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS actionlib roscpp rospy std_msgs tf)

## Declare a catkin package
catkin_package()
include_directories(include ${catkin_INCLUDE_DIRS})

## Build 
add_executable(make_plan src/make_plan.cpp)
target_link_libraries(make_plan ${catkin_LIBRARIES})
## add_dependencies(navigation_example ${catkin_EXPORTED_TARGETS})
