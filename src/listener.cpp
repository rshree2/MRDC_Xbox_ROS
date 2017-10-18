#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <base_controller/Xbox_Button_Msg.h>


int Global=0;
int Global_xbox=0;
void chatterCallback(const sensor_msgs::LaserScan::ConstPtr& ranges)
{
  float sum =0;
  float avrg;

  for (int i=0; i<21; i++)
  {
	  sum += ranges->ranges[i];
  }
  avrg = sum/20.0;
  if (avrg > float(0.3) && avrg < float(2.5))
  {
    Global=1;
  }

  ROS_INFO("The distance is: %g  cm", avrg);
}

void chatterCallback_xbox(const base_controller::Xbox_Button_Msg::ConstPtr& button)
{
 int xx= button->x;
   if(xx==1)
   {
     if (Global_xbox==0)
        {
          Global_xbox =1;
        }
     else 
	{
	  Global_xbox=0;
	}
   }
ROS_INFO("global value is: ", xx, Global_xbox);
}


int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "listener");

  
  ros::NodeHandle n;

  
  ros::Subscriber sub = n.subscribe("scan", 1000, chatterCallback);
  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
  ros::Subscriber sub_xbox = n.subscribe("xbox_controller", 1000, chatterCallback_xbox);
  ros::Rate loop_rate(1);   
 
  while (ros::ok())
  {
	  geometry_msgs::Twist msg;
	   if (Global == 1 && Global_xbox == 1)
	  {
		msg.linear.x = float(0.4);  
		chatter_pub.publish(msg);
		
      }
      Global =0;
      	
      
      ros::spinOnce();
      loop_rate.sleep();
  }

  
  ros::spin();

  return 0;
}
