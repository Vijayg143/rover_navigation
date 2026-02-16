# rover_navigation

https://github.com/user-attachments/assets/ddb4cded-d9e2-4bf6-bea5-ed3c4d612c32

[README.md](https://github.com/user-attachments/files/25336685/README.md)
source every terminal :- source install/setup.bash

completely cleans your ROS 2 workspace so you can do a fresh build :- rm -rf build log install

To launch gazebo  :-  ros2 launch k12_description gazebo.launch.py

To launch rviz   :-  ros2 launch k12_description display.launch.py

for moving robot :- ros2 run k12_description teleop_control

for mapping :- ros2 launch k12_description online_async.launch.py

to save the map :- ros2 run nav2_map_server map_saver_cli -f ~/roverr_ws/map

for navigation :- ros2 launch k12_description navigation.launch.py

to start RViz with Navigation2 (Nav2) configuration. :- ros2 launch nav2_bringup rviz_launch.py
