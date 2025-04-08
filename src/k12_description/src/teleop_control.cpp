#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <map>

class KeyboardTeleop : public rclcpp::Node
{
public:
    KeyboardTeleop()
    : Node("keyboard_teleop_node")
    {
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
        RCLCPP_INFO(this->get_logger(), "Keyboard Teleop Started! Use W/A/X/D to move,S to stop, Q to quit.");
        setup_terminal();
        run();
        reset_terminal();
    }

private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;

    std::map<char, std::pair<float, float>> move_bindings_ = {
        {'w', {1.0, 0.0}},   // forward
        {'x', {-1.0, 0.0}},  // backward
        {'a', {0.0, 1.0}},   // turn left
        {'d', {0.0, -1.0}}   // turn right
    };

    struct termios orig_term_attr;

    void setup_terminal()
    {
        struct termios new_term_attr;
        tcgetattr(fileno(stdin), &orig_term_attr);
        new_term_attr = orig_term_attr;
        new_term_attr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);
    }

    void reset_terminal()
    {
        tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);
    }

    char get_key()
    {
        char c;
        if (read(fileno(stdin), &c, 1) < 0)
        {
            perror("read():");
            exit(-1);
        }
        return c;
    }

    void run()
    {
        while (rclcpp::ok())
        {
            char key = get_key();
            auto twist = geometry_msgs::msg::Twist();

            if (move_bindings_.count(key))
            {
                twist.linear.x = move_bindings_[key].first * 0.5;   // Adjust speed
                twist.angular.z = move_bindings_[key].second * 1.0; // Adjust turn rate
                publisher_->publish(twist);
                RCLCPP_INFO(this->get_logger(), "Key: %c → linear.x=%.2f, angular.z=%.2f", key, twist.linear.x, twist.angular.z);
            }
            else if (key == 'q')
            {
                RCLCPP_INFO(this->get_logger(), "Quitting teleop...");
                break;
            }
            else
            {
                publisher_->publish(geometry_msgs::msg::Twist()); // stop if any other key
            }
        }

        // Stop the robot before exiting
        publisher_->publish(geometry_msgs::msg::Twist());
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    std::shared_ptr<KeyboardTeleop> node = std::make_shared<KeyboardTeleop>();
    rclcpp::shutdown();
    return 0;
}
