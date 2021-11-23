#include <ros/ros.h>
#include <std_msgs/String.h>
#include <actionlib_test/TaskAction.h> //(ROSパッケージ)/(acitionファイル名)Action.h
#include <actionlib/client/simple_action_client.h>
#include <iostream>

typedef actionlib::SimpleActionClient<actionlib_test::TaskAction> Client;

std_msgs::String task;
void string_callback(const std_msgs::String& message)
{
    task = message;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "task_client");
    ros::NodeHandle nh;

    ros::Subscriber string_sub = nh.subscribe("task", 10, string_callback);

    Client client("task", true);

    ros::Rate loop_rate(2);
    while(ros::ok())
    {
        if(client.isServerConnected()){
            if(task.data == "set"){
                actionlib_test::TaskGoal goal;
                goal.mode = true;
                goal.duration = 0;
                client.sendGoal(goal);
                std::cout<<"set true"<<std::endl;
            }else if(task.data == "unset"){
                actionlib_test::TaskGoal goal;
                goal.mode = false;
                goal.duration = 0;
                client.sendGoal(goal);
                std::cout<<"set false"<<std::endl;
            }else if(task.data == "set10s"){
                actionlib_test::TaskGoal goal;
                goal.mode = true;
                goal.duration = 10;
                client.sendGoal(goal);
                std::cout<<"set true after 10s"<<std::endl;
            }else if(task.data == "cancel"){
                client.cancelGoal();
                std::cout<<"publish cancel"<<std::endl;
            
            }else{
                std::cout << "invalid task!"<< std::endl;
            }
        }

        task.data = "";

        ros::spinOnce();
        loop_rate.sleep();
        
    }

    return 0;
}