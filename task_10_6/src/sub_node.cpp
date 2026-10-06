#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include "std_msgs/msg/string.hpp"
#include <string>  // 新增：用于拼接字符串

class SubNode : public rclcpp::Node {
public:
    SubNode() : Node("sub_node") {
        // 1. 订阅 pub_node 发过来的干净位移数据。
        sub_ = this->create_subscription<std_msgs::msg::Float64>(
            "/clean_displacement", 10,
            std::bind(&SubNode::callback, this, std::placeholders::_1));
        
        // 2. 创建发布者：发布包含数值的状态字符串给云台。
        pub_ = this->create_publisher<std_msgs::msg::String>("/gimbal_status", 10);
        
        RCLCPP_INFO(this->get_logger(), "Sub节点已启动！等待接收位移数据...");
    }

private:
    void callback(const std_msgs::msg::Float64::SharedPtr msg) {
        std_msgs::msg::String status_msg;
        
        // 核心逻辑：如果位移是0，发HOLD；如果大于0，把数字拼在TRACK后面发出去。
        if (msg->data == 0.0) {
            status_msg.data = "HOLD";
        } else {
            // 将浮点数转换为字符串，并拼接到 "TRACK: " 后面
            status_msg.data = "TRACK: " + std::to_string(msg->data);
        }
        
        pub_->publish(status_msg);
    }
    
    rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr sub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SubNode>());
    rclcpp::shutdown();
    return 0;
}