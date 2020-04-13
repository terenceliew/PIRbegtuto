#include "ros/ros.h"
#include "std_msgs/String.h"
#include <visualization_msgs/Marker.h>
#include <iostream>
#include <sstream>

//Global variable
unsigned int value ;

ros::Publisher marker_pub;
uint32_t shape;



/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  uint32_t cpt=0;
  
  //ROS_INFO("I heard: [%s]", msg->data.c_str());
  //sstream::stringstream s(msg->data.c_str()) >> double_value ;
  std::istringstream myStream (msg->data.c_str());
  //double_value = reinterpret_cast<unsigned int>(msg->data.c_str()) ;
  myStream >> value ;
  //double_value = 2*double_value ;
  std::cout << "The value recieved is " << value << std::endl ;
  //if (double_value == 200){ std::cout << "PIR_ROS is on fire" << std::endl ; } ;

  //Now create a marker and send it to rviz

  visualization_msgs::Marker marker;
  // Set the frame ID and timestamp.  See the TF tutorials for information on these.
  marker.header.frame_id = "/my_frame";
  marker.header.stamp = ros::Time::now();

  // Set the namespace and id for this marker.  This serves to create a unique ID
  // Any marker sent with the same namespace and id will overwrite the old one
  marker.ns = "basic_shapes";
  marker.id = 0;

  // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
  marker.type = shape;

  // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
  marker.action = visualization_msgs::Marker::ADD;

  // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
  if(cpt==0){
    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
    cpt++;
  }else{
  marker.pose.position.x = value/3.6;
  marker.pose.position.y = 0;
  marker.pose.position.z = 0;
  marker.pose.orientation.x = 0.0;
  marker.pose.orientation.y = 0.0;
  marker.pose.orientation.z = 0.0;
  marker.pose.orientation.w = 1.0;
  }

  // Set the scale of the marker -- 1x1x1 here means 1m on a side
  marker.scale.x = 1.0;
  marker.scale.y = 1.0;
  marker.scale.z = 1.0;

  // Set the color -- be sure to set alpha to something non-zero!
  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;

  marker.lifetime = ros::Duration();

  // Publish the marker
  while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
	{
	  return 0;
	}
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
  marker_pub.publish(marker);
}


int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "listener");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;
  ros::Rate r(0.5);

  //Used to send marker to RVIZ
  marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  //We display a cube
  shape = visualization_msgs::Marker::CUBE;


  while (ros::ok())
  {
  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
    ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
  
  /*if (cpt == 0 ) {
    ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
    coord_x=double_value;
    cpt = 1;
    std::cout << "coord x : " << coord_x << std::endl ;
  }
  else {
    ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
    coord_y=double_value;
    cpt = 0;
    std::cout << "coord y : " << coord_y << std::endl ;
  }*/

  }
  
  return 0;
}
