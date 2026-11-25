cmake_minimum_required(VERSION 3.8)
project(entri_ros2_pubsub_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# Generate the custom message interface
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/RobotStatus.msg"
)

ament_export_dependencies(rosidl_default_runtime)

# Publisher
add_executable(publisher_node src/publisher_node.cpp)
ament_target_dependencies(publisher_node rclcpp std_msgs)

# Subscriber
add_executable(subscriber_node src/subscriber_node.cpp)
ament_target_dependencies(subscriber_node rclcpp std_msgs)

# Let both nodes resolve the custom message from this same package.
# Jazzy and newer: rosidl_get_typesupport_target (rosidl_target_interfaces is deprecated).
# Humble and older: rosidl_target_interfaces.
if(COMMAND rosidl_get_typesupport_target)
  rosidl_get_typesupport_target(cpp_typesupport_target
    ${PROJECT_NAME} "rosidl_typesupport_cpp")
  target_link_libraries(publisher_node "${cpp_typesupport_target}")
  target_link_libraries(subscriber_node "${cpp_typesupport_target}")
else()
  rosidl_target_interfaces(publisher_node ${PROJECT_NAME} "rosidl_typesupport_cpp")
  rosidl_target_interfaces(subscriber_node ${PROJECT_NAME} "rosidl_typesupport_cpp")
endif()

install(TARGETS
  publisher_node
  subscriber_node
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
