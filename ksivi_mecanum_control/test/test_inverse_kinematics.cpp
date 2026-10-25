#include <gtest/gtest.h>

#include "ksivi_mecanum_control/mecanum_inverse_kinematics.hpp"

namespace ksivi_mecanum_control {
namespace {

constexpr double kRadius = 0.1;
constexpr double kWheelTrack = 0.4;
constexpr double kWheelBase = 0.5;
constexpr double kEpsilon = 1e-9;

}  // namespace

TEST(MecanumInverseKinematics, ForwardDrivesAllWheelsEqually) {
  // Arrange
  MecanumInverseKinematics kinematics(kRadius, kWheelTrack, kWheelBase);

  // Act
  const WheelVelocities wheel_velocities =
      kinematics.ComputeWheelVelocities({1.0, 0.0, 0.0});

  // Assert
  EXPECT_NEAR(wheel_velocities.front_left, 10, kEpsilon);
  EXPECT_NEAR(wheel_velocities.front_right, 10, kEpsilon);
  EXPECT_NEAR(wheel_velocities.back_left, 10, kEpsilon);
  EXPECT_NEAR(wheel_velocities.back_right, 10, kEpsilon);
}

TEST(MecanumInverseKinematics, ClockwiseRotation) {
  // Arrange
  MecanumInverseKinematics kinematics(kRadius, kWheelTrack, kWheelBase);

  // Act
  const WheelVelocities wheel_velocities =
      kinematics.ComputeWheelVelocities({0.0, 0.0, -1.0});

  // Assert
  EXPECT_NEAR(wheel_velocities.front_left, 4.5, kEpsilon);
  EXPECT_NEAR(wheel_velocities.front_right, -4.5, kEpsilon);
  EXPECT_NEAR(wheel_velocities.back_left, 4.5, kEpsilon);
  EXPECT_NEAR(wheel_velocities.back_right, -4.5, kEpsilon);
}

TEST(MecanumInverseKinematics, CounterClockwiseRotation) {
  // Arrange
  MecanumInverseKinematics kinematics(kRadius, kWheelTrack, kWheelBase);

  // Act
  const WheelVelocities wheel_velocities =
      kinematics.ComputeWheelVelocities({0.0, 0.0, 1.0});

  // Assert
  EXPECT_NEAR(wheel_velocities.front_left, -4.5, kEpsilon);
  EXPECT_NEAR(wheel_velocities.front_right, 4.5, kEpsilon);
  EXPECT_NEAR(wheel_velocities.back_left, -4.5, kEpsilon);
  EXPECT_NEAR(wheel_velocities.back_right, 4.5, kEpsilon);
}

}  // namespace ksivi_mecanum_control
