cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_COMPILER clang++)

project(ksivi_mecanum_control)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-4")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(std_msgs REQUIRED)

# Set KSIVI_MECANUM_CONTROL_CORE_SOURCE
set(
  KSIVI_MECANUM_CONTROL_CORE_SOURCE
  src/mecanum_controller_node.cpp
  src/mecanum_inverse_kinematics.cpp
)


add_library(
  ${PROJECT_NAME}_core
  STATIC
  ${KSIVI_MECANUM_CONTROL_CORE_SOURCE}
)

target_include_directories(
  ${PROJECT_NAME}_core
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_executable(
  ${PROJECT_NAME}
  src/main.cpp
)

ament_target_dependencies(
  ${PROJECT_NAME}_core
  rclcpp
  geometry_msgs
  rcl_interfaces
  std_msgs
)

target_link_libraries(
  ${PROJECT_NAME}
  ${PROJECT_NAME}_core
)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(
    test_inverse_kinematics
    test/test_inverse_kinematics.cpp
  )

  target_link_libraries(
    test_inverse_kinematics
    ${PROJECT_NAME}_core
  )

  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  # uncrustify требует ROS-стиль и конфликтует с Google-стилем (.clang-format)
  set(ament_cmake_uncrustify_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()

