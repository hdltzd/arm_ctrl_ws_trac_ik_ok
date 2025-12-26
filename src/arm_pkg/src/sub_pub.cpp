// #include "ros/ros.h"
// #include "std_msgs/UInt16MultiArray.h"
// #include <serial/serial.h>
// #include "arm_pkg/sbus_parse.h"
// #include <iostream>
// #include "arm_pkg/handle_info.h"
// #include <std_msgs/Float64MultiArray.h>
// #include <vector>
// #include "canbus_driver.h"
// CanbusDriver can_driver;
// class CANdata{
//     public:
//         int32_t id;
//         int32_t byte;
//         int64_t goal_pos;
//     //设置id，字节，目标
//     void setid (int a){
//         id=a;
//         ROS_INFO("MY ID is %d",id);
//     }
//     void setbyte (){
//         byte=7;
//         ROS_INFO("MY byte is %d",byte);
//     }
//     void setgoal (double joint_angle){
//         goal_pos=(654784.0 / (90.0 - 54.3878))*joint_angle+(1000000.0 - (654784.0 / (90.0 - 54.3878)) * 54.3878);
//         ROS_INFO_STREAM("MY POS is "<<goal_pos);
//     }
// };
// std::vector<double> dealed_joint_angles(6, 0.0);
// bool got_new_data = false;
// std::vector<CANdata>my_CANdata_array(6);
// void callback(const arm_pkg::handle_info::ConstPtr& msg) {
//         // 1. 变量设置
//         //模式切换
//         int model_button = msg->left_putter_a;// 上1 回到起点运动  中2 关节控制启动
//         //速度切换
//         int speed_button = msg->right_putter_b;// 上1速度10%  中2速度30%  下3速度80%

//         //2.映射规则
//         if(model_button==2 && msg->connected){//处于关节控制模式且链接正常

//             ROS_INFO("joint controller model is running...");

//             dealed_joint_angles[0]=(msg->left_x / 100.0) * 180.0;//基座关节旋转 左摇杆x轴 -100～100 ->-180～180

//             dealed_joint_angles[1]=(msg->left_y / 100.0) * 90.0;//肩部俯仰 左摇杆y轴 -100～100 ->-90～90

//             dealed_joint_angles[2]=(msg->right_x / 100.0) * 90.0;//肘部俯仰 右摇杆x轴 -100～100 ->-90～90

//             dealed_joint_angles[3]=(msg->right_y / 100.0) * 180.0;//腕部旋转 右边摇杆y轴 -100～100 ->-180～180

//             dealed_joint_angles[4]=(0.75 * msg->left_button_vra) - 90.0;//腕部俯仰 VRA旋钮  0～240 ->-90～90

//             dealed_joint_angles[5]= (0.75 * msg->right_button_vrb) - 90.0;//夹爪开合距离 VRA旋钮  0～240 ->-90~90
//             ROS_INFO("OP OF JOINT ANGLE:\n joint1: %.2f \t joint2: %.2f \t joint3: %.2f\t joint4: %.2f\t joint5: %.2f\t joint6: %.2f",dealed_joint_angles[0],dealed_joint_angles[1],dealed_joint_angles[2],dealed_joint_angles[3],dealed_joint_angles[4],dealed_joint_angles[5]);
//         }
//         if(model_button==1){
//             std::fill(dealed_joint_angles.begin(), dealed_joint_angles.end(), 0);
//             ROS_INFO("moving to start...");
//         }
//         double joint_speed =1;
//         switch (speed_button)//设置关节运动速度
//         {
//         case 1:
//             joint_speed =1.1;
//             break;
//         case 2:
//             joint_speed =1.3;
//             break;
//          case 3:
//             joint_speed =1.8;
//             break;
//         default:
//             break;
//         }
//          ROS_INFO("crent speed:%.2f",joint_speed);
//         //3.关节角度安全限制
//         double min_max_joints[7][2]={//关节限制矩阵
//             {-180,180},
//             {-90,90},
//             {-90,90},
//             {-180,180},
//             {-90,90},
//             {-90,90}
//         };
//         dealed_joint_angles[0] = std::max(min_max_joints[0][0], std::min(min_max_joints[0][1], dealed_joint_angles[0]));
//         dealed_joint_angles[1] = std::max(min_max_joints[1][0], std::min(min_max_joints[1][1], dealed_joint_angles[1]));
//         dealed_joint_angles[2] = std::max(min_max_joints[2][0], std::min(min_max_joints[2][1], dealed_joint_angles[2]));
//         dealed_joint_angles[3] = std::max(min_max_joints[3][0], std::min(min_max_joints[3][1], dealed_joint_angles[3]));
//         dealed_joint_angles[4] = std::max(min_max_joints[4][0], std::min(min_max_joints[4][1], dealed_joint_angles[4]));
//         dealed_joint_angles[5] = std::max(min_max_joints[5][0], std::min(min_max_joints[5][1], dealed_joint_angles[5]));
//         ROS_INFO("fainl JOINT ANGLE:\n joint1: %.2f \t joint2: %.2f \t joint3: %.2f\t joint4: %.2f\t joint5: %.2f\t joint6: %.2f",dealed_joint_angles[0],dealed_joint_angles[1],dealed_joint_angles[2],dealed_joint_angles[3],dealed_joint_angles[4],dealed_joint_angles[5]);
//         got_new_data = true;
//         ROS_INFO("save latest joint angles");
    
//     for(int i=0;i<6;i++){
//         my_CANdata_array[i].setid(i);
//         my_CANdata_array[i].setbyte();
//         my_CANdata_array[i].setgoal(dealed_joint_angles[i]);
//         ROS_INFO("joint:%d's data alreday change to CANdata: joint_id:%d\t joint_byte:%d\t joint2pos:%lld",i,my_CANdata_array[i].id,my_CANdata_array[i].byte,my_CANdata_array[i].goal_pos);
//     }
        
// }
// void CANdata2motordata (std::vector<CANdata> & data1){
//     uint8_t id[6]={0};
//     int32_t pos[6]={0};
//     uint16_t max_speed[6]={1000};
//     for(int i=0;i<6;i++){
//     uint8_t id[i]={ data1[i].id};
//     int32_t pos[i]={data1[i].goal_pos};
//     }
//     ROS_INFO("CANdata alreday change to motordata");
// }
    

// int main(int argc,char *argv[]){
//     setlocale(LC_ALL,"");
//     ros::init(argc,argv,"sub_pub");
//     ros::NodeHandle nh;
//     ros::Subscriber sub_from_tele =nh.subscribe<arm_pkg::handle_info>("handle_info", 15,callback);
//     CANdata2motordata(my_CANdata_array);

//     ros::spin();
//     return 0;
// }

