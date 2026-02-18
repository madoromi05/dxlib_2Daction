#pragma once
#include "DxLib.h"
#include "ResorcePath.h"
class Map;

class Character {
public:
    Character();
    void Initialize();
    void Update(class Map* map);
    void Draw() const;

private:
    int m_playerX;
    int m_playerY;
    // アニメーション定数（画像に合わせて調整してください）
    static constexpr int kIdleFrameCount = 6;  // 画像のコマ数
    static constexpr int kAnimSpeed = 8;       // アニメーションの速度（大きいほど遅い）

    // 画像ハンドル配列（コマ数分用意する）
    int m_idleHandles[kIdleFrameCount];

    // アニメーション管理用
    int m_animTimer;      // 時間計測用
    int m_currentFrame;   // 現在表示しているコマ番号(0 ~ 5)

    static constexpr int kChipWidth = 128;
    static constexpr int kChipHeight = 128;
    static constexpr int kDrawWidth = 128;
    static constexpr int kDrawHeight = 128;
};