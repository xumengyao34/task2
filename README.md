这个package主要是解决TF坐标在不断抖动的问题，筛选静止状态下较小抖动，不再转给云台。
一、数据流
节点一（pub_node）订阅TF，实现筛选，传给topic（clean_displacement），节点二（sub_node）订阅该话题，判断是否行动，并传给topic(gimbal_status)，后续由云台订阅。
如图
<img width="2756" height="1710" alt="topic" src="https://github.com/user-attachments/assets/878dc1f7-80bd-41d8-8160-a24639335e96" />

二、操作
终端一 launch启动
source install/setup.bash 
ros2 launch task_10_6 task_launch.py
终端二 打开回放
ros2 bag play rosbag…
终端三  订阅topic，检验结果
ros2 topic echo /gimbal_status
三、AI使用
代码均为AI编写，框架由我构建。
过程截图与运行节点录屏如下
<img width="2392" height="1194" alt="rosbag" src="https://github.com/user-attachments/assets/3941f4ce-fa56-4268-9179-4951b8b559d6" />

<img width="1544" height="1158" alt="rviz" src="https://github.com/user-attachments/assets/2c65183b-7cd2-4c7f-b5ef-586da8b14729" />

[frames_2026-10-06_11.09.05.pdf](https://github.com/user-attachments/files/33105656/frames_2026-10-06_11.09.05.pdf)

[Screencast from 2026年10月06日 20时02分37秒.webm](https://github.com/user-attachments/assets/2076678c-b547-45ee-8eb0-4afb5a792bce)
当然，这个也存在一定弊端性，只依据了TF极短时间下位移来实现筛选。可考虑采用多方面如里程计，IMU三者共同判断。
