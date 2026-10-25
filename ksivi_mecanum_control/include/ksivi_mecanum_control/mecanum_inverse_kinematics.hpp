#ifndef KSIVI_MECANUM_CONTROL_MECANUM_CONTROLLER_NODE_HPP_
#define KSIVI_MECANUM_CONTROL_MECANUM_CONTROLLER_NODE_HPP_

#include <geometry_msgs/msg/twist.hpp>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>

#include "ksivi_mecanum_control/mecanum_inverse_kinematics.hpp"

namespace ksivi_mecanum_control {

class MecanumControllerNode : public rclcpp::Node {
 public:
  MecanumControllerNode();

 private:
  void CmdVelSubscriptionCallback(geometry_msgs::msg::Twist);
  void SendToMotorSpeedPublisher(const ChassisVelocity& vel);

  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr
      cmd_vel_subscription_;
  rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr
      motor_speed_publisher_;

  rclcpp::TimerBase::SharedPtr motor_speed_timer_;
  ChassisVelocity last_velocity_;
};

using MecanumControllerNodePtr = std::shared_ptr<MecanumControllerNode>;
MecanumControllerNodePtr MakeMecanumControllerNode();

}  // namespace ksivi_mecanum_control

#endif  // KSIVI_MECANUM_CONTROL_MECANUM_CONTROLLER_NODE_HPP_
