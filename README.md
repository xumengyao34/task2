# 导航方向第二次任务：TF简单抖动过滤与状态监控

## 一、意义

主要构思是解决TF坐标在不断抖动的问题，筛选出静止状态下较小抖动，不会将其转给云台。

## 二、数据流

节点一（pub_node）订阅TF，实现筛选，发送到话题clean_displacement，节点二（sub_node）订阅该话题，判断是否行动，将指令与对应位置改变发送到话题gimbal_status，后续该话题应由云台订阅。

主要框架如图
<img width="2756" height="1710" alt="topic" src="https://github.com/user-attachments/assets/878dc1f7-80bd-41d8-8160-a24639335e96" />

## 三、话题与参数
### 话题

|话题名称|作用|
|---|---|
|/tf|输入|
|/clean_displacement|输出过滤后信息|
|/gimbal_status|输出状态与改变|

### 参数

movement_threshold 

默认值为0.01,由初步观察所录数据得出，后续应观测实际情况更改


## 四、操作

### 终端一 launch启动

```bash
colcon build --packages-select

source install/setup.bash

ros2 launch task_10_6 task_launch.py
```

### 终端二 接收对应数据

在这里作示范的是本人当时录的包
```bash
ros2 bag play rosbag2_2026_10_05-18_07_53 --loop
```

### 终端三  订阅topic，检验结果

```bash
source install/setup.bash

ros2 topic echo /gimbal_status
```

## 五、AI使用

代码均为AI编写，框架与对应数据情况分析等由本人完成。

## 六、过程截图与运行节点录屏
<img width="2392" height="1194" alt="rosbag" src="https://github.com/user-attachments/assets/3941f4ce-fa56-4268-9179-4951b8b559d6" />

<img width="1544" height="1158" alt="rviz" src="https://github.com/user-attachments/assets/2c65183b-7cd2-4c7f-b5ef-586da8b14729" />

<img width="1900" height="1214" alt="TF" src="https://github.com/user-attachments/assets/22f7373b-3680-48c2-aa47-4a53856c3d5f" />


[Screencast from 2026年10月06日 20时02分37秒.webm](https://github.com/user-attachments/assets/2076678c-b547-45ee-8eb0-4afb5a792bce)

## 七、反思

当然，这个也存在一定弊端性，只依据了TF极短时间下位移来实现筛选，极大可能会忽略到缓慢移动，导致结果不准确。后续可考虑采用多方面如里程计，IMU三者共同判断。

当时我在实验室录的包没怎么动雷达，所以我的数据也有局限性，可能没有完全测试到它的可行性。
