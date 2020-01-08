#include "control.h"
#include <ros/ros.h>


int main(int argc, char** argv)
{
  // init ROS node
  

  // get private parameters
 

  // init publishers and messages
 

  // init TF listener
 

  const double dt(0.05);
  ros::Rate loop(1/dt);
  double x1, x2, y1, y2;
  while(ros::ok())
  {
    // get transforms
  

    // compute command
    const auto cmd = command(x1, y1, x2, y2);

    // build and publish twist message
   

    // build and publish joint state message
  

    ros::spinOnce();
    loop.sleep();
  }



}
