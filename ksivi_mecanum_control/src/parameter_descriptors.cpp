#include "ksivi_mecanum_control/parameter_descriptors.hpp"

namespace ksivi_mecanum_control {

ParameterDescriptorType GetDescriptorWheelRadius() {
  constexpr double kLower = 0.01;
  constexpr double kUpper = 1;
  auto parameter_descriptor = rcl_interfaces::msg::ParameterDescriptor();
  parameter_descriptor.description = R"(
- Единица измерения: м. (метры)
- Описание:
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

ParameterDescriptorType GetDescriptorWheelBase() {
  constexpr double kLower = 0.01;
  constexpr double kUpper = 10;
  auto parameter_descriptor = rcl_interfaces::msg::ParameterDescriptor();
  parameter_descriptor.description = R"(
- Единица измерения: м. (метры)
- Описание:
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

ParameterDescriptorType GetDescriptorWheelTrack() {
  constexpr double kLower = 0.01;
  constexpr double kUpper = 10;
  auto parameter_descriptor = rcl_interfaces::msg::ParameterDescriptor();
  parameter_descriptor.description = R"(
- Единица измерения: м. (метры)
- Описание:
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

}  // namespace ksivi_mecanum_control