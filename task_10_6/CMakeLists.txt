cmake_minimum_required(VERSION 3.8)
project(task_10_6)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)

add_executable(pub_node src/pub_node.cpp)
ament_target_dependencies(pub_node rclcpp std_msgs geometry_msgs tf2_ros tf2_geometry_msgs)

add_executable(sub_node src/sub_node.cpp)
ament_target_dependencies(sub_node rclcpp std_msgs)

install(TARGETS
  pub_node
  sub_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
