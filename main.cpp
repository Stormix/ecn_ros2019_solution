#include "control.h"
#include <ros/ros.h>
#include <string>
#include <tf/transform_listener.h>
#include "sensor_msgs/JointState.h"
#include "geometry_msgs/Twist.h"
#include <geometry_msgs/TransformStamped.h>

std::vector<std::string> jointNames = {"wheel", "torso", "neck"};

int main(int argc, char **argv)
{
  // init ROS node
  ros::init(argc, argv, "control");
  ros::NodeHandle nh("~");

  std::string robot = "d0";
  std::string friend_ = "bb8";
  std::string foe = "bb9";

  // get private parameters
  nh.getParam("robot", robot);
  nh.getParam("friend", friend_);
  nh.getParam("foe", foe);

  // init publishers and messages
  ros::Publisher cmdPub = nh.advertise<geometry_msgs::Twist>("/" + robot + "/cmd_vel", 1);
  ros::Publisher statePub = nh.advertise<sensor_msgs::JointState>("/" + robot + "/joint_states", 1);
  
  // *About the topic names*: 
  // When using the namespace and relative path for the topic names (ie: "cmd_vel" and "joint_states"), 
  // the publishers don't publish in the correct topic "/namespace/cmd_vel" for instance, they publish in /namespace/control/cmd_vel
  // I couldn't figure out why in time, so I opted for the solution above.
  
  // Another option I tried to avoid using the robot variable and use topic names "cmd_vel" & "joint_states", 
  // was to remove the namespace and name the nodes in the launch file as "d0" and "d9" respectively,
  // but the rqt_graph doesn't match the one provided unfortunely.
  
  // The current setup however, matches the given graph perfectly.

  // init TF listener
  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener(tfBuffer);
  geometry_msgs::TransformStamped transformFriend;
  geometry_msgs::TransformStamped transformFoe;

  const double dt(0.05);
  ros::Rate loop(1 / dt);
  double x1, x2, y1, y2;

  // init joint command
  sensor_msgs::JointState jointCmd;
  jointCmd.name = jointNames;
  jointCmd.position.resize(3, 0);

  while (ros::ok())
  {
    // get transforms
    try
    {
      transformFriend = tfBuffer.lookupTransform(robot, friend_, ros::Time(0));
      transformFoe = tfBuffer.lookupTransform(robot, foe, ros::Time(0));
    }
    catch (tf2::TransformException &ex)
    {
      ROS_WARN("%s", ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }

    // get positions
    x1 = transformFriend.transform.translation.x;
    y1 = transformFriend.transform.translation.y;
    x2 = transformFoe.transform.translation.x;
    y2 = transformFoe.transform.translation.y;

    // compute command
    const auto cmd = command(x1, y1, x2, y2);

    // build and publish twist message
    geometry_msgs::Twist twistCmd;

    twistCmd.linear.x = cmd.v;
    twistCmd.angular.z = cmd.w;

    cmdPub.publish(twistCmd);

    // build and publish joint state message
    jointCmd.header.stamp = ros::Time::now();

    jointCmd.position[0] += 3.7 * cmd.v * dt;
    jointCmd.position[1] = cmd.v * M_PI / 12;
    jointCmd.position[2] = cmd.w * M_PI / 12;

    statePub.publish(jointCmd);

    ros::spinOnce();
    loop.sleep();
  }
}
