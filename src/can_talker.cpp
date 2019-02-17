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
    ros::NodeHandle n;
    ros::Publisher can_tx_pub = n.advertise<ros_lawicel_canusb::CanMessage>("can_tx", 1000);
    ros::Rate loop_rate(10);
    while(ros::ok())
    {
        ros_lawicel_canusb::CanMessage msg;
        std::stringstream ss;
        ss << "a1234567 1 1 8";
        msg.data = ss.str();
        ROS_INFO("%s", msg.data.c_str());
        can_tx.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
