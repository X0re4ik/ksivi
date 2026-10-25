#include <iostream>

#include "ksivi_mecanum_control/mecanum_controller_node.hpp"

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(ksivi_mecanum_control::MakeMecanumControllerNode());
  rclcpp::shutdown();
  return 0;
}
