# VEX 2026 基础框架

## 当前状态

依赖：PROS kernel 4.2.1 / LemLib 0.5.6 / liblvgl 9.2.0。
参考去年提供的 `main.cpp`，保留当前分文件结构。

`include/robot_config.hpp` 中的 `ROBOT_ENABLE_CHASSIS` 默认是 **0**。
此时不创建电机、IMU、Rotation、TrackingWheel 或 Chassis 对象，不校准、不启动定位任务，
也不会发送底盘指令。自动和手动入口会显示未配置提示并返回。
这是无硬件框架，不是仿真器，不会生成模拟位置。

## 文件职责

| 文件 | 内容 |
| --- | --- |
| `src/main.cpp` | PROS 比赛回调，调度初始化、自动、手动和停止 |
| `include/robot_config.hpp` | 硬件开关、端口、底盘尺寸、轮速、追踪轮偏移和刷新周期 |
| `include/robot.hpp` | 上层使用的底盘接口 |
| `src/robot.cpp` | 硬件对象、PID、摇杆曲线、LemLib chassis、校准、定位显示和停止 |
| `include/routines.hpp` | 自动与手动入口声明 |
| `src/autonomous.cpp` | 自动动作顺序；目前仅有注释示例，不执行路线 |
| `src/driver.cpp` | 左摇杆 Y 前后、右摇杆 X 转向，10 ms 更新一次 |
| `include/main.h` | PROS 原生回调声明 |

依赖方向：比赛入口 → 自动/手动流程 → 硬件接口 → PROS/LemLib。
电机和传感器对象只在 `robot.cpp` 定义，避免多个文件重复创建同一设备。

## 从去年模板沿用的部分

- 左右电机组、IMU、一纵一横两个 Rotation 追踪轮的配置结构。
- LemLib 的直线/转向 PID、里程计、摇杆指数曲线和 arcade 手动控制。
- 初始化时校准；启用并初始化成功后，每 50 ms 在 Brain 屏幕显示一次位置。
- PID 参考值仍为直线 `10 / 0 / 3`、转向 `2 / 0 / 10`，曲线参考值为 `3 / 10 / 1.019`。
  它们不是今年机器人的调试结果。

去年模板注释写的是 curvature，但实际调用的是 `chassis.arcade()`，这里统一按 arcade 命名。
没有启用旧模板的演示自动路线、`ASSET(example_txt)` 或连续位置日志。
项目中的 `static/example.txt` 保留作参考，目前代码不调用它。

旧硬件参数仅作查阅，**没有填入今年配置**：

| 参数 | 去年模板 |
| --- | --- |
| 左 / 右电机端口 | `{-5, 4, -3}` / `{6, -9, 7}` |
| 电机齿轮盒 / IMU | blue / 10 |
| 纵向 / 横向 Rotation 端口 | -11 / 20 |
| 驱动轮 / 左右轮间距 / 轮速 | `Omniwheel::NEW_4` / 10 英寸 / 360 rpm |
| 追踪轮 | `Omniwheel::NEW_275` |
| 纵向 / 横向偏移 | -2.5 / -5.75 英寸 |

配置和 API 可对照 [LemLib 0.5.6 文档](https://lemlib.readthedocs.io/en/v0.5.6/api/)。
以项目自带的 `include/lemlib` 头文件为接口依据。

## 自动程序写法

当前自动程序保持空闲。确认硬件、定位方向和调参后，可以在 `src/autonomous.cpp` 中编写：

```cpp
robot::drive::setPose(0, 0, 0);
robot::drive::moveToPoint(0, 12, 2000);
robot::drive::turnToHeading(90, 1500);
```

坐标单位为英寸、角度为度、超时为毫秒；这里的点是绝对坐标。
起点和朝向应与实际摆放位置一致，`setPose()` 只设定定位信息，不移动机器人。
这两个运动接口内部使用 LemLib 的同步模式：前一个动作结束或超时，才执行下一个。
返回并不保证已经到达目标。无硬件或初始化失败时，这些接口不执行动作。

后续确实需要 `moveToPose`、路径跟随或动作重叠时，再按需求扩展底盘接口。
机构按 intake、lift 等实际功能增加模块；手动循环中避免长时间等待。

## 接硬件时

1. 按今年实际结构填写 `robot_config.hpp`：电机数量、端口和反转、齿轮盒、轮径、轮间距、轮速、传感器端口和偏移。
   轮速是传动后驱动轮的转速；轮径应采用对应轮型的实际直径。
2. 当前硬件分支沿用“一纵一横追踪轮 + IMU”。如果今年布局不同，先调整 `robot.cpp` 中的 `TrackingWheel` 和 `OdomSensors`。
3. 配置完成并接好硬件后，将 `ROBOT_ENABLE_CHASSIS` 改为 1。
   缺少端口、端口重复/越界、齿轮盒未配置或尺寸未填写时，编译会报错。
4. 启动时保持静止完成校准。`ready()` 表示初始化通过，不是持续故障监测，也不代表 PID 已调好。
5. 验证电机方向、定位变化、模式切换时停止，再调 PID、摇杆曲线，最后启用自己的自动路线。

`robot::stop()` 会取消底盘动作并输出零；新增机构后也应在这里加入机构停止操作。

## 编译与验证

在 PROS 终端执行 `pros make`，或在完整 ARM 工具链已经加入 `PATH` 时执行 `make -j4`。
本机 VS Code 安装的工具链目录为：

```text
/Users/leo/Library/Application Support/Code/User/globalStorage/sigbots.pros/install/pros-toolchain-macos/bin
```

编译和链接只能验证代码与库兼容；尚未验证实机运动、校准效果或比赛模式切换。
