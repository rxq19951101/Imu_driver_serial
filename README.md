# Imu_driver_serial

I am using stm32 as MCU to pass the data from imu to my robot.\
But i found there is no driver for stm32 to ROS. I decide to write by myself.\
These are the hardwares all i have:\
MCU---STM32F103\
IMU---MPU9250(with mag)\
\
\
If you want to use this script, you have to install serial lib\
and type the following codes on your terminal\
$git clone URL  \
$sudo apt-get install ros-kinetic-serial  \
$cd ~/catkin_make  \
$catkin_make
