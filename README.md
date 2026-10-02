# VEX 2026 基础框架

依赖：PROS kernel 4.2.1 / LemLib 0.5.6 / liblvgl 9.2.0。
参考去年模板，按文件拆分，使用普通函数名和直接的 LemLib 调用。

## 文件职责

| 文件 | 内容 |
| --- | --- |
| `src/main.cpp` | PROS 比赛入口，调用 initializeRobot、runAutonomous、runDriver、stopRobot |
| `include/routines.hpp` | runAutonomous、runDriver 的声明 |
| `src/autonomous.cpp` | 自动动作顺序 |
| `src/driver.cpp` | 手柄输入，直接调用 chassis.arcade |
| `include/robot.hpp` | 用 extern 声明 chassis，以及初始化、停止函数 |
| `src/robot.cpp` | 定义硬件和 chassis，配置 PID、摇杆曲线，执行初始化、停止和定位显示 |
| `include/robot_config.hpp` | 端口、齿轮盒、底盘尺寸、轮速等常量 |

`extern lemlib::Chassis chassis;` 是声明，不会创建另一个底盘。
实际对象只在 `robot.cpp` 中定义一次。其他文件包含 `robot.hpp` 后直接使用它。
项目自己的函数和配置不再放在自定义命名空间中；pros:: 和 lemlib:: 是库本身的命名空间。

## 电机配置

| 侧别 | 端口 | 齿轮盒 | 方向 |
| --- | --- | --- | --- |
| 左 | 17、18 | blue | 正转 |
| 左 | 19 | green | 反转 |
| 右 | 1 | green | 反转 |
| 右 | 2、20 | blue | 正转 |

齿轮盒列表与对应的端口列表顺序一一对应，初始化时分别设置。
最终 driveWheelRpm 需要结合绿色、蓝色电机各自的外部传动比确定。
20 号端口已分配给右侧电机，不能再给传感器使用。

传感器端口：IMU 为 10，横向 Rotation 为 3，纵向 Rotation 为 16。
两个 Rotation 暂按不反转配置，安装后确认读数方向。

## 待填写参数

轮径、轮间距、轮速仍有 0 占位值，追踪轮偏移也需要按实物测量填写。
当前暂沿用“一纵一横 Rotation 追踪轮 + IMU”的结构；实际布局不同时需修改 robot.cpp。
PID 和摇杆曲线保留去年的参考值，需要重新调试。

已移除 ROBOT_ENABLE_CHASSIS、static_assert、端口/齿轮盒校验、ready 和硬件可用性检查。
程序启动后直接设置电机齿轮盒并校准 chassis，不再提供无硬件模式。
**请先填完真实配置再上传运行；编译成功不代表配置正确。**

## 自动和手动

自动动作直接写在 `runAutonomous()` 内，目前示例保持注释：

```cpp
chassis.setPose(0, 0, 0);
chassis.moveToPoint(0, 12, 2000, {}, false);
chassis.turnToHeading(90, 1500, {}, false);
```

坐标用英寸、角度用度、超时用毫秒。setPose 设定定位信息，不会移动机器人。
最后的 false 表示同步执行，等动作结束或超时才继续；不保证已经到达目标。
省略该参数会使用 LemLib 默认的异步模式。

手动使用左摇杆 Y 前后、右摇杆 X 转向，每 10 ms 更新一次。
禁用时调用 stopRobot，取消底盘动作并输出零。新增机构后在这里补充机构停止代码。

## 编译

在 PROS 终端执行 `pros make`，或把完整 ARM 工具链加入 PATH 后执行 `make -j4`。
本机工具链目录：

```text
/Users/leo/Library/Application Support/Code/User/globalStorage/sigbots.pros/install/pros-toolchain-macos/bin
```

未进行实机运动、校准或比赛模式切换验证。
