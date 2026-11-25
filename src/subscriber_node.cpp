#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "entri_ros2_pubsub_cpp/msg/robot_status.hpp"

using std::placeholders::_1;

class MinimalSubscriber : public rclcpp::Node
{
public:
  MinimalSubscriber()
  : Node("minimal_subscriber")
  {
    subscription_ = this->create_subscription<entri_ros2_pubsub_cpp::msg::RobotStatus>(
      "robot_status", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
  }

private:
  void topic_callback(const entri_ros2_pubsub_cpp::msg::RobotStatus::SharedPtr msg) const
  {
    RCLCPP_INFO(
      this->get_logger(), "Robot %d is at %.1f%% battery. Status: %s",
      msg->robot_id, msg->battery_level, msg->status_message.c_str());
  }

  rclcpp::Subscription<entri_ros2_pubsub_cpp::msg::RobotStatus>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}
