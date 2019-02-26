//
//  can_talker.cpp
//  
//
//  Created by Chensk on 2019/2/17.
//
#include <sstream>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "ros/ros.h"
#include "ros_lawicel_canusb/CanMessage.h"
#include <geometry_msgs/TwistStamped.h>
static double g_current_velocity;
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}
static void currentVelCallback(const geometry_msgs::TwistStampedConstPtr &msg)
{
    g_current_velocity = msg->twist.linear.x;
    ROS_INFO("I heard: [%d]", g_current_velocity);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "can_talker");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("twist_cmd",1000,currentVelCallback);
    ros::Publisher can_tx_pub = n.advertise<ros_lawicel_canusb::CanMessage>("can_tx", 1000);
    ros::Rate loop_rate(10);
    while(ros::ok())
    {
        ros_lawicel_canusb::CanMessage msg;
        msg.data={255, 2, 3, 4, 5, 6, 7, 'a'};
        msg.id=123;
        msg.extended=1;
        msg.dlc=8;
        can_tx_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
