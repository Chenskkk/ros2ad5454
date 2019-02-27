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
char tmp_linear[8];
char tmp_angular[8];


static void TwistCallback(const geometry_msgs::TwistStampedConstPtr &msg)
{
    static double linear_speed;
    static double angular_speed;
    angular_speed = msg->twist.angular.z;
    linear_speed = msg->twist.linear.x;
    ROS_INFO("linear: [%f], angular:[%f]", linear_speed,angular_speed);
    
    sprintf(tmp_linear, "%f", linear_speed);
    sprintf(tmp_angular, "%f", angular_speed);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "can_talker");
    ros::NodeHandle n;
    ros::Subscriber sub_twist_cmd = n.subscribe("twist_cmd",1000,TwistCallback);
    ros::Publisher can_tx_pub = n.advertise<ros_lawicel_canusb::CanMessage>("can_tx", 1000);
    ros::Rate loop_rate(10);
    while(ros::ok())
    {
        ros_lawicel_canusb::CanMessage msg;
        msg.data={tmp_linear[0], tmp_linear[1] , tmp_linear[2] , tmp_linear[3] , tmp_linear[4], tmp_linear[5], tmp_linear[6], tmp_linear[7]};
        msg.id=100;
        msg.extended=1;
        msg.dlc=8;
        can_tx_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
