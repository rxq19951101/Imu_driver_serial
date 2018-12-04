#include <ros/ros.h> 
#include <nodelet/nodelet.h>
#include <serial/serial.h>  //ROS已经内置了的串口包 
#include <std_msgs/String.h> 
#include <std_msgs/Empty.h> 
#include <string>
#include <sstream>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>
template <class Type>
Type stringToNum(const std::string& str)
{
	std::istringstream iss(str);
	Type num;
	iss >> num;
	return num;    
}

  
serial::Serial ser; //声明串口对象 
  
//回调函数 
void write_callback(const std_msgs::String::ConstPtr& msg) 
{ 
    ROS_INFO_STREAM("Writing to serial port" <<msg->data); 
    ser.write(msg->data);   //发送串口数据 
} 
  
int main (int argc, char** argv) 
{   static int count=0;
    static int Seq=0,here=0;
    sensor_msgs::Imu HelloRoboto_imu;
    sensor_msgs::MagneticField HelloRoboto_mag;
    float acc[3];    
    float ang[3];
    int mag[3];
    std::string s1,s2,s3;
    //初始化节点 
    ros::init(argc, argv, "imunode"); 
    //声明节点句柄 
    ros::NodeHandle nh; 
  
    //订阅主题，并配置回调函数 
    ros::Subscriber write_sub = nh.subscribe("write", 1000, write_callback); 
    //发布主题 
    ros::Publisher pub_imu = nh.advertise<sensor_msgs::Imu>("Hellorobot_Imu", 1000); 
    ros::Publisher pub_mag = nh.advertise<sensor_msgs::MagneticField>("Mag",1);
//    NODELET_INFO_STREAM("Advertized on topic 9axis-imu");
//    NODELET_INFO_STREAM("Advertized on topic Mag");
    try 
    { 
    //设置串口属性，并打开串口 
        ser.setPort("/dev/ttyUSB0"); 
        ser.setBaudrate(115200); 
        serial::Timeout to = serial::Timeout::simpleTimeout(1500); 
        ser.setTimeout(to); 
        ser.open(); 
    } 
    catch (serial::IOException& e) 
    { 
        ROS_ERROR_STREAM("Unable to open port "); 
        return -1; 
    } 
  
    //检测串口是否已经打开，并给出提示信息 
    if(ser.isOpen()) 
    { 
        ROS_INFO_STREAM("Serial Port initialized"); 
    } 
    else 
    { 
        return -1; 
    } 
    ser.write("s");              //向串口发送's'代表开始传输数据
    //指定循环的频率 
    ros::Rate loop_rate(2000);
    std::string swap;
    while(ros::ok()) 
    { 
        int n2,n3;
        if(ser.available()){ 
            ROS_INFO_STREAM("Reading from serial port\n"); 
            std_msgs::String result;
            std::string s;
            std::cout<<here++<<std::endl;
  //          result.data = ser.read(ser.available());
        ser.readline(result.data);
        s=result.data;
        ROS_INFO_STREAM(result.data);
        if(s[0]=='a'){
            s1=s.substr(1,12);   //将每一个acc的值从buffer中取出来
            swap=s.substr(1,30);
            n2=swap.find('a');
            std::cout<<swap<<std::endl;
            n3=s.rfind('a');
            s2=s.substr(n2+2,12);
            std::cout<<s2<<std::endl;
            s3=s.substr(n3+1,12);
            acc[0]=stringToNum<float>(s1);
            acc[1]=stringToNum<float>(s2);
            acc[2]=stringToNum<float>(s3);
           // std::cout << acc << std::endl;
            count++;
        }
        if(s[0]=='b'){
            s1=s.substr(1,12);   //将每一个ang的值从buffer中取出来
            swap=s.substr(1,30);
            n2=swap.find('b');
            n3=s.rfind('b');
            s2=s.substr(n2+2,12);
            
            s3=s.substr(n3+1,12);
            ang[0]=stringToNum<float>(s1);
            ang[1]=stringToNum<float>(s2);
            ang[2]=stringToNum<float>(s3);
            count++;
        }
        if(s[0]=='c'){
            s1=s.substr(1,5);   //将每一个mag的值从buffer中取出来
            swap=s.substr(1,30);
            n2=swap.find('c');
            n3=s.rfind('c');
            s2=s.substr(n2+2,5);
            s3=s.substr(n3+1,5);
            mag[0]=stringToNum<int>(s1);
            mag[1]=stringToNum<int>(s2);
            mag[2]=stringToNum<int>(s3);
            count++;
        }
        if(count==3){
            count=0;
            Seq++;
            HelloRoboto_imu.header.seq = Seq;
            HelloRoboto_mag.header.seq = Seq;

            HelloRoboto_imu.linear_acceleration.x=acc[0];
            HelloRoboto_imu.linear_acceleration.y=acc[1];
            HelloRoboto_imu.linear_acceleration.z=acc[2];
    
    HelloRoboto_imu.linear_acceleration_covariance[0] = 0;
    HelloRoboto_imu.linear_acceleration_covariance[1] = 0;
    HelloRoboto_imu.linear_acceleration_covariance[2] = 0;

    HelloRoboto_imu.linear_acceleration_covariance[3] = 0;
    HelloRoboto_imu.linear_acceleration_covariance[4] = 0;
    HelloRoboto_imu.linear_acceleration_covariance[5] = 0;

    HelloRoboto_imu.linear_acceleration_covariance[6] = 0;
    HelloRoboto_imu.linear_acceleration_covariance[7] = 0;
    HelloRoboto_imu.linear_acceleration_covariance[8] = 0;
    
            HelloRoboto_imu.angular_velocity.x=ang[0];
            HelloRoboto_imu.angular_velocity.y=ang[1];
            HelloRoboto_imu.angular_velocity.z=ang[2];

    HelloRoboto_imu.angular_velocity_covariance[0] = 0;
    HelloRoboto_imu.angular_velocity_covariance[1] = 0;
    HelloRoboto_imu.angular_velocity_covariance[2] = 0;

    HelloRoboto_imu.angular_velocity_covariance[3] = 0;
    HelloRoboto_imu.angular_velocity_covariance[4] = 0;
    HelloRoboto_imu.angular_velocity_covariance[5] = 0;

    HelloRoboto_imu.angular_velocity_covariance[6] = 0;
    HelloRoboto_imu.angular_velocity_covariance[7] = 0;
    HelloRoboto_imu.angular_velocity_covariance[8] = 0;

            HelloRoboto_mag.magnetic_field.x=mag[0];
            HelloRoboto_mag.magnetic_field.y=mag[1];
            HelloRoboto_mag.magnetic_field.z=mag[2];

    HelloRoboto_mag.magnetic_field_covariance[0] = 0; 
    HelloRoboto_mag.magnetic_field_covariance[1] = 0;  
    HelloRoboto_mag.magnetic_field_covariance[2] = 0;  

    HelloRoboto_mag.magnetic_field_covariance[3] = 0;  
    HelloRoboto_mag.magnetic_field_covariance[4] = 0;  
    HelloRoboto_mag.magnetic_field_covariance[5] = 0;  
    
    HelloRoboto_mag.magnetic_field_covariance[6] = 0;  
    HelloRoboto_mag.magnetic_field_covariance[7] = 0;  
    HelloRoboto_mag.magnetic_field_covariance[8] = 0;         
        
 //       result.data= s.substr(0,20);
 //       ROS_INFO_STREAM(result.data[0]); 

//          ROS_INFO_STREAM("Read: " << result.data); 
            pub_imu.publish(HelloRoboto_imu);
            pub_mag.publish(HelloRoboto_mag);
        } 
        }

//处理ROS的信息，比如订阅消息,并调用回调函数 
        ros::spinOnce(); 
        loop_rate.sleep(); 
  
    } 

}

