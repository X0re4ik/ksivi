#include "ksivi_mecanum_control/mecanum_inverse_kinematics.hpp"

#include <memory>

namespace ksivi_mecanum_control {

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
MecanumInverseKinematics::MecanumInverseKinematics(double wheel_radius,
                                                   double wheel_track,
                                                   double wheel_base)
    : wheel_radius_(wheel_radius),
      wheel_track_(wheel_track),
      wheel_base_(wheel_base),
      half_track_(wheel_track_ / 2.0),
      half_wheelbase_(wheel_base_ / 2.0),
      rotation_lever_arm_(half_wheelbase_ + half_track_) {}

WheelVelocities MecanumInverseKinematics::ComputeWheelVelocities(
    const ChassisVelocity& chassis_velocity) const {
  const double linear_x = chassis_velocity.vx;
  const double linear_y = chassis_velocity.vy;
  const double angular_z = chassis_velocity.wz;

  WheelVelocities wheel_velocities;
  wheel_velocities.front_left =
      (linear_x + linear_y - rotation_lever_arm_ * angular_z) / wheel_radius_;
  wheel_velocities.front_right =
      (linear_x - linear_y + rotation_lever_arm_ * angular_z) / wheel_radius_;
  wheel_velocities.back_left =
      (linear_x - linear_y - rotation_lever_arm_ * angular_z) / wheel_radius_;
  wheel_velocities.back_right =
      (linear_x + linear_y + rotation_lever_arm_ * angular_z) / wheel_radius_;

  return wheel_velocities;
}

MecanumInverseKinematicsPtr MakeMecanumInverseKinematics(double wheel_radius,
                                                         double wheel_track,
                                                         double wheel_base) {
  return std::make_unique<MecanumInverseKinematics>(wheel_radius, wheel_track,
                                                    wheel_base);
}

}  // namespace ksivi_mecanum_control
