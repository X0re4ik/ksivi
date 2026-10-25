#include "ksivi_mecanum_control/mecanum_controller_node.hpp"

#include <chrono>
#include <memory>
#include <vector>

#include "ksivi_mecanum_control/mecanum_inverse_kinematics.hpp"
#include "ksivi_mecanum_control/parameter_descriptors.hpp"

namespace ksivi_mecanum_control {

constexpr int kDefaultQoS = 10;
constexpr std::chrono::milliseconds kMotorSpeedPeriod{50};
constexpr double kDefaultWheelRadius = 0.05;
constexpr double kDefaultWheelTrack = 0.25;
constexpr double kDefaultWheelBase = 0.25;

MecanumControllerNode::MecanumControllerNode()
    : rclcpp::Node("mecanum_controller"),
      cmd_vel_subscription_(
          this->create_subscription<geometry_msgs::msg::Twist>(
              "/cmd_vel", kDefaultQoS,
              [this](geometry_msgs::msg::Twist msg) {
                CmdVelSubscriptionCallback(msg);
              })),
      motor_speed_publisher_(
          this->create_publisher<std_msgs::msg::Float32MultiArray>(
              "/motor_speeds", kDefaultQoS)),
      motor_speed_timer_(this->create_wall_timer(kMotorSpeedPeriod, [this] {
        SendToMotorSpeedPublisher(last_velocity_);
      })) {
  this->declare_parameter("wheel_radius", kDefaultWheelRadius,
                          GetDescriptorWheelRadius());
  this->declare_parameter("wheel_track", kDefaultWheelTrack,
                          GetDescriptorWheelTrack());
  this->declare_parameter("wheel_base", kDefaultWheelBase,
                          GetDescriptorWheelBase());

  auto wheel_radius = this->get_parameter("wheel_radius").as_double();
  auto wheel_track = this->get_parameter("wheel_track").as_double();
  auto wheel_base = this->get_parameter("wheel_base").as_double();

  mecanum_inverse_kinematics_ =
      MakeMecanumInverseKinematics(wheel_radius, wheel_track, wheel_base);

  last_velocity_ = ChassisVelocity{.vx = 0, .vy = 0, .wz = 0};
}

void MecanumControllerNode::SendToMotorSpeedPublisher(
    const ChassisVelocity& vel) {
  constexpr int kThrottle = 5000;

  auto wheel_velocities =
      mecanum_inverse_kinematics_->ComputeWheelVelocities(vel);

  // Send Array float values with speed wheels to /motor_speeds
  // Порядок колёс в Float32MultiArray: [FL, FR, BL, BR]
  std_msgs::msg::Float32MultiArray float_array_msg;
  std::vector<float> data = {static_cast<float>(wheel_velocities.front_left),
                             static_cast<float>(wheel_velocities.front_right),
                             static_cast<float>(wheel_velocities.back_left),
                             static_cast<float>(wheel_velocities.back_right)};

  RCLCPP_INFO_THROTTLE(
      this->get_logger(), *this->get_clock(), kThrottle,
      "front_left=%f;front_right=%f;back_left=%f;back_right=%f", data[0],
      data[1], data[2], data[3]);

  float_array_msg.set__data(data);
  motor_speed_publisher_->publish(float_array_msg);
}

void MecanumControllerNode::CmdVelSubscriptionCallback(
    geometry_msgs::msg::Twist msg) {
  last_velocity_ = ChassisVelocity{
      .vx = msg.linear.x, .vy = msg.linear.y, .wz = msg.angular.z};
}

MecanumControllerNodePtr MakeMecanumControllerNode() {
  return std::make_shared<MecanumControllerNode>();
}

}  // namespace ksivi_mecanum_control