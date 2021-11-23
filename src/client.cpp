#include <ros/ros.h>
#include <actionlib_test/TaskAction.h> //(ROSパッケージ)/(acitionファイル名)Action.h
#include <actionlib/server/simple_action_server.h>

typedef actionlib::SimpleActionServer<actionlib_test::TaskAction> Server;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "task_server");
    ros::NodeHandle nh;

    Server server(nh, "task", false);
    server.start();

    ros::Time startTime;
    
    ros::Rate loop_rate(2);
    actionlib_test::TaskGoalConstPtr current_goal;
    while(ros::ok())
    {
        if(server.isNewGoalAvailable()){
            current_goal = server.acceptNewGoal();
            startTime = ros::Time::now();
            std::cout<<"update goal"<<std::endl;
        }

        if(server.isActive()){
            if(server.isPreemptRequested()){
                server.setPreempted();
                std::cout<<"preempt goal"<<std::endl;
            }else{
                if(startTime + ros::Duration(current_goal->duration) < ros::Time::now()){
                    server.setSucceeded();
                    std::cout<<"task Succeeded!"<<std::endl;
                }else{
                    actionlib_test::TaskFeedback feedback;
                    feedback.rate = (ros::Time::now() - startTime).toSec() / current_goal->duration;
                    server.publishFeedback(feedback);
                }
            }

            if(current_goal->mode){
                std::cout<<"True"<<std::endl;
            }else{
                std::cout<<"False"<<std::endl;
            }
        }
        
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}