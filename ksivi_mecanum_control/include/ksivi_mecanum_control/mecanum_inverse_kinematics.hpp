#ifndef KSIVI_MECANUM_CONTROL_MECANUM_INVERSE_KINEMATICS_HPP_
#define KSIVI_MECANUM_CONTROL_MECANUM_INVERSE_KINEMATICS_HPP_

#include <memory>

namespace ksivi_mecanum_control {

struct ChassisVelocity {
  double vx{0.0};
  double vy{0.0};
  double wz{0.0};
};

struct WheelVelocities {
  double front_left{0.0};
  double front_right{0.0};
  double back_left{0.0};
  double back_right{0.0};
};

class MecanumInverseKinematicsInterface {
 public:
  MecanumInverseKinematicsInterface() = default;
  MecanumInverseKinematicsInterface(const MecanumInverseKinematicsInterface&) =
      default;
  MecanumInverseKinematicsInterface(MecanumInverseKinematicsInterface&&) =
      default;
  MecanumInverseKinematicsInterface& operator=(
      const MecanumInverseKinematicsInterface&) = default;
  MecanumInverseKinematicsInterface& operator=(
      MecanumInverseKinematicsInterface&&) = default;

  virtual ~MecanumInverseKinematicsInterface() = default;

  [[nodiscard]] virtual WheelVelocities ComputeWheelVelocities(
      const ChassisVelocity& chassis_velocity) const = 0;
};

class MecanumInverseKinematics : public MecanumInverseKinematicsInterface {
 public:
  MecanumInverseKinematics(double wheel_radius, double wheel_track,
                           double wheel_base);

  [[nodiscard]] WheelVelocities ComputeWheelVelocities(
      const ChassisVelocity& chassis_velocity) const override;

 private:
  double wheel_radius_;
  double wheel_track_;
  double wheel_base_;

  double half_track_;
  double half_wheelbase_;
  double rotation_lever_arm_;
};

using MecanumInverseKinematicsPtr =
    std::unique_ptr<MecanumInverseKinematicsInterface>;

MecanumInverseKinematicsPtr MakeMecanumInverseKinematics(double wheel_radius,
                                                         double wheel_track,
                                                         double wheel_base);

}  // namespace ksivi_mecanum_control

#endif  // KSIVI_MECANUM_CONTROL_MECANUM_INVERSE_KINEMATICS_HPP_
