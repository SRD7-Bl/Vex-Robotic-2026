#pragma once

// Override 场地坐标表：只存场地位置，运动指令写在 autonomous.cpp。
// 参考：官方 Appendix A，填写时对照 Field Element Locations 图。
// https://manuals.vex.com/v5/override/v2.1/appendix-a/
//  坐标约定（从图中的 Audience View 看场地）：
//   原点：官方尺寸图的左下角零点。
//   +X：向右；+Y：向上；单位：英寸。
//   名称中的 Top / Bottom / Left / Right 均相对于这张图。

struct FieldPoint {
    float x;
    float y;
};

// ----------- 特殊点 ---------------
inline constexpr FieldPoint leftStartPoint{7.5f, 70.2f};
inline constexpr FieldPoint rightStartPoint{132.91f, 70.2f};
inline constexpr FieldPoint topStartPoint{70.2f, 132.91f};
inline constexpr FieldPoint bottomStartPoint{70.31f, 7.5f};

// ----------- Goal ---------------
// 红方
inline constexpr FieldPoint redGoalLeft{23.11f, 46.66f};
inline constexpr FieldPoint redGoalBottom{46.66f, 23.11f};

// 蓝方
inline constexpr FieldPoint blueGoalTop{93.75f, 117.3f};      
inline constexpr FieldPoint blueGoalRight{117.3f, 93.75f};    

// 中立
inline constexpr FieldPoint neutralGoalTop{46.66f, 117.3f};     // 左上区域靠上
inline constexpr FieldPoint neutralGoalLeft{23.11f, 93.75f};    // 左上区域靠左
inline constexpr FieldPoint neutralGoalRight{117.3f, 46.66f};   // 右下区域靠右
inline constexpr FieldPoint neutralGoalBottom{93.75f, 23.11f};  // 右下区域靠下
inline constexpr FieldPoint midfieldGoal{70.2f, 70.2f};         // 场地正中

// ----------- Loader ---------------
inline constexpr FieldPoint redLoaderTop{0.0f, 0.0f};           // 左上
inline constexpr FieldPoint redLoaderBottom{0.0f, 0.0f};        // 左下
inline constexpr FieldPoint blueLoaderTop{0.0f, 0.0f};          // 右上
inline constexpr FieldPoint blueLoaderBottom{0.0f, 0.0f};       // 右下

// ----------- Toggle ---------------
inline constexpr FieldPoint toggleTop{70.2f, 140.41f};          // 上边墙
inline constexpr FieldPoint toggleBottom{70.2f, 0.0f};          // 下边墙
inline constexpr FieldPoint toggleLeft{0.0f, 70.2f};            // 左边墙
inline constexpr FieldPoint toggleRight{140.41f, 70.2f};        // 右边墙

// ----------- Pin/Cup ---------------
//Yellow-Yellow
inline constexpr FieldPoint YYLeftBottom{23.11f, 23.11f};       // 左下
inline constexpr FieldPoint YYLeftBottomCenter{46.66f, 46.66f}; // 左下偏中心
inline constexpr FieldPoint YYRightTop{117.3f, 117.3f};         // 右上
inline constexpr FieldPoint YYRightTopCenter{93.75f, 93.75f};   // 右上偏中心

//Blue-Red
inline constexpr FieldPoint BRLeft{46.66f, 70.2f};              
inline constexpr FieldPoint BRRight{93.75f, 70.2f};
inline constexpr FieldPoint BRTop{70.2f, 93.75f};
inline constexpr FieldPoint BRBottom{70.2f, 46.66f};

//周围围着4个Pin的
inline constexpr FieldPoint cupLeftTop{23.11f, 117.3f};
inline constexpr FieldPoint cupLeftTopCenter{46.66f, 93.75f};
inline constexpr FieldPoint cupRightBottom{117.3f, 23.11f};
inline constexpr FieldPoint cupRightBottomCenter{93.75f, 46.66f};

//边上的Cup
//左下
inline constexpr FieldPoint cupLeftBottomSideBottom{1.58f, 43.5f};
inline constexpr FieldPoint cupLeftBottomSideMiddle{1.58f, 46.66f};
inline constexpr FieldPoint cupLeftBottomSideTop{1.58f, 49.82f};

//左上
inline constexpr FieldPoint cupLeftTopSideBottom{1.58f, 90.59f};
inline constexpr FieldPoint cupLeftTopSideMiddle{1.58f, 93.75f};
inline constexpr FieldPoint cupLeftTopSideTop{1.58f, 96.91f};

//右下
inline constexpr FieldPoint cupRightBottomSideBottom{138.83f, 43.5f};
inline constexpr FieldPoint cupRightBottomSideMiddle{138.83f, 46.66f};
inline constexpr FieldPoint cupRightBottomSideTop{138.83f, 49.82f};

//右上
inline constexpr FieldPoint cupRightTopSideBottom{138.83f, 90.59f};
inline constexpr FieldPoint cupRightTopSideMiddle{138.83f, 93.75f};
inline constexpr FieldPoint cupRightTopSideTop{138.83f, 96.91f};

//下左
inline constexpr FieldPoint cupBottomLeftSideLeft{43.5f, 1.58f};
inline constexpr FieldPoint cupBottomLeftSideMiddle{46.66f, 1.58f};
inline constexpr FieldPoint cupBottomLeftSideRight{49.82f, 1.58f};

//下右
inline constexpr FieldPoint cupBottomRightSideLeft{90.59f, 1.58f};
inline constexpr FieldPoint cupBottomRightSideMiddle{93.75f, 1.58f};
inline constexpr FieldPoint cupBottomRightSideRight{96.91f, 1.58f};

//上左
inline constexpr FieldPoint cupTopLeftSideLeft{43.5f, 138.83f};
inline constexpr FieldPoint cupTopLeftSideMiddle{46.66f, 138.83f};
inline constexpr FieldPoint cupTopLeftSideRight{49.82f, 138.83f};

//上右
inline constexpr FieldPoint cupTopRightSideLeft{90.59f, 138.83f};
inline constexpr FieldPoint cupTopRightSideMiddle{93.75f, 138.83f};
inline constexpr FieldPoint cupTopRightSideRight{96.91f, 138.83f};
