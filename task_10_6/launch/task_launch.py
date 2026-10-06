from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='task_10_6',
            executable='pub_node',
            name='pub_node',
            parameters=[{'movement_threshold': 0.01}]
        ),
        Node(
            package='task_10_6',
            executable='sub_node',
            name='sub_node'
        )
    ])