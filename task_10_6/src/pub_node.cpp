#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "std_msgs/msg/float64.hpp"
#include <cmath>

class PubNode : public rclcpp::Node {
public:
    PubNode() : Node("pub_node") {
        // 1. 声明参数：位移阈值，默认 0.01 米。如果超过这个值，才认为是真的移动。
        this->declare_parameter("movement_threshold", 0.01);
        threshold_ = this->get_parameter("movement_threshold").as_double();

        // 2. 初始化 TF 监听器：用来读取 camera_init 到 body 的坐标变化。
        tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

        // 3. 创建发布者：把处理后的干净位移发给 sub_node。
        pub_ = this->create_publisher<std_msgs::msg::Float64>("/clean_displacement", 10);
        
        // 初始化变量：记录上一帧的位置，第一次进来的时候把 is_first_ 设为 true。
        last_x_ = 0.0; last_y_ = 0.0;
        is_first_ = true;

        // 4. 定时器：每 100 毫秒（10Hz）检查一次 TF 数据。
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&PubNode::timer_callback, this));

        RCLCPP_INFO(this->get_logger(), "Pub节点已启动，滤波阈值：%.4f", threshold_);
    }

private:
    void timer_callback() {
        try {
            // 尝试去获取当前的 TF 坐标。如果没拿到，会抛出异常（被下面 catch 捕获）。
            auto transform = tf_buffer_->lookupTransform("camera_init", "body", tf2::TimePointZero);
            double cur_x = transform.transform.translation.x;
            double cur_y = transform.transform.translation.y;

            // 如果是程序启动的第一帧，那就先记录位置，不计算差值。
            if (is_first_) {
                last_x_ = cur_x; last_y_ = cur_y;
                is_first_ = false;
                return;
            }

            // 核心计算：算一下当前位置和上一次位置的距离差（勾股定理）。
            double diff = std::sqrt(std::pow(cur_x - last_x_, 2) + std::pow(cur_y - last_y_, 2));
            std_msgs::msg::Float64 msg;

            // 核心逻辑：位移差小于阈值，说明是高频抖动，强制发 0；大于阈值，说明是真移动，原样发出去。
            if (diff < threshold_) {
                msg.data = 0.0;
                RCLCPP_INFO(this->get_logger(), "判定为静止抖动：强制归零");
            } else {
                msg.data = diff;
                RCLCPP_INFO(this->get_logger(), "判定为真实移动：输出位移 %.4f", diff);
            }
            
            // 更新记录的上一次位置，然后发布数据。
            last_x_ = cur_x; last_y_ = cur_y;
            pub_->publish(msg);

        } catch (tf2::TransformException &ex) {
            // 如果当前拿不到 TF 数据，什么都不做，跳过这一帧。
        }
    }

    double threshold_;
    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    double last_x_, last_y_;
    bool is_first_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PubNode>());
    rclcpp::shutdown();
    return 0;
}