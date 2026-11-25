#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "entri_ros2_pubsub_cpp/msg/robot_status.hpp"

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
  : Node("minimal_publisher"), battery_level_(100.0f)
  {
    publisher_ = this->create_publisher<entri_ros2_pubsub_cpp::msg::RobotStatus>(
      "robot_status", 10);
    timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    auto message = entri_ros2_pubsub_cpp::msg::RobotStatus();
    message.robot_id = 12;
    message.battery_level = battery_level_;
    message.status_message = "Operating";

    RCLCPP_INFO(this->get_logger(), "Publishing status for Robot %d", message.robot_id);
    publisher_->publish(message);

    // Decay battery continuously
    battery_level_ -= 0.5f;
    if (battery_level_ < 0.0f) {
      battery_level_ = 0.0f;
    }
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<entri_ros2_pubsub_cpp::msg::RobotStatus>::SharedPtr publisher_;
  float battery_level_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}
