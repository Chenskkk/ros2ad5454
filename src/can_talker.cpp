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
unsigned char tmp[8];
void FloatPut(float Fdat,unsigned char *Buf,unsigned char Pos)
{
    unsigned char *p;
    
    p = (unsigned char *)&Fdat;
    Buf[Pos] = *p;
    Buf[Pos+1] = *(p+1);
    Buf[Pos+2] = *(p+2);
    Buf[Pos+3] = *(p+3);
    Buf[Pos+4] = *(p+4);
    Buf[Pos+5] = *(p+5);
    Buf[Pos+6] = *(p+6);
    Buf[Pos+7] = *(p+7);
    
}

static void TwistCallback(const geometry_msgs::TwistStampedConstPtr &msg)
{
    static double linear_speed;
    static double angular_speed;
    angular_speed = msg->twist.angular.z;
    linear_speed = msg->twist.linear.x;
    FloatPut(linear_speed,tmp,0);
    ROS_INFO("linear: [%u], angular:[%f]", tmp[0],angular_speed);
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
        msg.data={tmp[0], tmp[1] , tmp[2] , tmp[3] , tmp[4], tmp[5], tmp[6], tmp[7]};
        msg.id=100;
        msg.extended=1;
        msg.dlc=8;
        can_tx_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
