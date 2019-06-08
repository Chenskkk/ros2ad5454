#!/bin/bash
echo ubuntu | rosrun ros_lawicel_canusb canopen.sh
gnome-terminal -x roscore
sleep 1
gnome-terminal -x bash -c 'roslaunch ros_lawicel_canusb lawicel_canusb.launch;exec bash'
#echo ubuntu | sudo -S ros