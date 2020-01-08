cmake_minimum_required(VERSION 2.8.3)
project(ecn_ros2019)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  tf2_ros
)

catkin_package()



include_directories(${catkin_INCLUDE_DIRS})

add_executable(control main.cpp)
target_link_libraries(control ${catkin_LIBRARIES})
