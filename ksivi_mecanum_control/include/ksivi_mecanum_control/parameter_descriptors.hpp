#ifndef KSIVI_MECANUM_CONTROL_PARAMETER_DESCRIPTORS_HPP_
#define KSIVI_MECANUM_CONTROL_PARAMETER_DESCRIPTORS_HPP_

#include <rcl_interfaces/msg/parameter_descriptor.hpp>

namespace ksivi_mecanum_control {

using ParameterDescriptorType = rcl_interfaces::msg::ParameterDescriptor;

ParameterDescriptorType GetDescriptorWheelRadius();
ParameterDescriptorType GetDescriptorWheelBase();
ParameterDescriptorType GetDescriptorWheelTrack();

}  // namespace ksivi_mecanum_control

#endif  // KSIVI_MECANUM_CONTROL_PARAMETER_DESCRIPTORS_HPP_
