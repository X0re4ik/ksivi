#include "ksivi_mecanum_control/mecanum_controller_node.hpp"

#include <chrono>
#include <memory>
#include <vector>

#include "ksivi_mecanum_control/mecanum_inverse_kinematics.hpp"

namespace ksivi_mecanum_control {

namespace {

constexpr int kDefaultQoS = 10;
constexpr std::chrono::milliseconds kMotorSpeedPeriod{50};

class DescriptorFactory {
 public:
  static auto GetRadius() {
    constexpr double kLower = 0.01;
    constexpr double kUpper = 10;
    auto parameter_descriptor = rcl_interfaces::msg::ParameterDescriptor();
    parameter_descriptor.description = R"(
      Вид сбоку

      .─────────.
  .´             `.
  /                 \
│        O──────────●
  \          radius /
  `.             .´
      '─────────'

      O — центр колеса
    )";
    parameter_descriptor.floating_point_range = {
        rcl_interfaces::msg::FloatingPointRange()
            .set__from_value(kLower)
            .set__to_value(kUpper)};
    return parameter_descriptor;
  }

  static auto GetWheelBase() {
    constexpr double kLower = 0.01;
    constexpr double kUpper = 10;
    auto parameter_descriptor = rcl_interfaces::msg::ParameterDescriptor();
    parameter_descriptor.description = R"(
          ↑ Вперёд

Левый борт       Правый борт

    [■]─────────────[■]  ─┐
      Передняя ось        │
                          │
                          │
            O             │  wheel_base
                          │
                          │
      Задняя ось          │
    [■]─────────────[■]  ─┘

          Вид сверху
    )";
    parameter_descriptor.floating_point_range = {
        rcl_interfaces::msg::FloatingPointRange()
            .set__from_value(kLower)
            .set__to_value(kUpper)};
    return parameter_descriptor;
  }

  static auto GetWheelTrack() {
    constexpr double kLower = 0.01;
    constexpr double kUpper = 10;
    auto parameter_descriptor = rcl_interfaces::msg::ParameterDescriptor();
    parameter_descriptor.description = R"(
          ↑ Вперёд

Левый борт       Правый борт

    [■]─────────────[■]
      │               │
      │       O       │
      │               │
    [■]─────────────[■]
      │               │
      └───────────────┘
         wheel_track

          Вид сверху
    )";
    parameter_descriptor.floating_point_range = {
        rcl_interfaces::msg::FloatingPointRange()
            .set__from_value(kLower)
            .set__to_value(kUpper)};
    return parameter_descriptor;
  }
};

}  // namespace

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
  constexpr double kDefaultRadius = 0.05;
  this->declare_parameter("wheel_radius", kDefaultRadius,
                          DescriptorFactory::GetRadius());

  constexpr double kDefaultWheelTrack = 0.25;
  this->declare_parameter("wheel_track", kDefaultWheelTrack,
                          DescriptorFactory::GetWheelTrack());

  constexpr double kDefaultWheelBase = 0.25;
  this->declare_parameter("wheel_base", kDefaultWheelBase,
                          DescriptorFactory::GetWheelBase());

  last_velocity_ = ChassisVelocity{.vx = 0, .vy = 0, .wz = 0};
}

void MecanumControllerNode::SendToMotorSpeedPublisher(
    const ChassisVelocity& vel) {
  // Getting parameters machine
  auto radius = this->get_parameter("wheel_radius").as_double();
  auto wheel_track = this->get_parameter("wheel_track").as_double();
  auto wheel_base = this->get_parameter("wheel_base").as_double();

  constexpr int kThrottle = 5000;
  RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), kThrottle,
                       "wheel_radius=%f;wheel_track=%f;wheel_base=%f", radius,
                       wheel_track, wheel_base);
  // Calculate speed wheels
  auto mecanum_inverse_kinematics =
      MakeMecanumInverseKinematics(radius, wheel_track, wheel_base);
  auto wheel_velocities =
      mecanum_inverse_kinematics->ComputeWheelVelocities(vel);

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