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
#include "data2msg.h"
#include "ros_lawicel_canusb/CanMessage.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "can_talker");
    ros::NodeHandle k;
    ros::Publisher can_chatter_pub = k.advertise<ros_lawicel_canusb::CanMessage>("can_chatter", 1000);
    ros::Rate loop_rate(10);
    int count = 0;
    while(ros::ok())
    {
        ros_lawicel_canusb::CanMessage msg;
        std::stringstream ss;
        ss << "a1234567 1 1 8";
        msg.data = ss.str();
        ROS_INFO("%s", msg.data.c_str());
        can_chatter_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
