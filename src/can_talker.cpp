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
unsigned char tmp[4];
void FloatPut(float Fdat,unsigned char *Buf,unsigned char Pos)
{
    unsigned char *p;
    
    p = (unsigned char *)&Fdat;
    Buf[Pos] = *p;
    Buf[Pos+1] = *(p+1);
    Buf[Pos+2] = *(p+2);
    Buf[Pos+3] = *(p+3);
}

static void TwistCallback(const geometry_msgs::TwistStampedConstPtr &msg)
{
    static double linear_speed;
    static double angular_speed;
    angular_speed = msg->twist.angular.z;
    linear_speed = msg->twist.linear.x;
    FloatPut(linear_speed,tmp,0);
    ROS_INFO("linear: [%f], angular:[%f]", linear_speed,angular_speed);
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
        msg.data={tmp[2], 2, 3, 4, 5, 6, 7, 'a'};
        msg.id=100;
        msg.extended=1;
        msg.dlc=8;
        can_tx_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
