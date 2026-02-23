#pragma once
#include "DrowInterface/DrawableList.h"

class Map;

class Character : public IDrawable{
public:
    Character();
    void Initialize();
    void Update(class Map* map);
    void Draw() const override;
	int GetY() const { return m_playerY; }
    int m_playerSpeed = 3;

private:
    void Move(class Map* map, int key);
    void Jump(int key);
    void Gravity(class Map* map);

    float m_playerX;
    float m_playerY;
    float m_nextX;
    float m_nextY;
    float m_velocityY;

    //重力
    bool m_isGrounded;
    static constexpr float kGravity   = 0.98f;
    static constexpr float kJumpPower = -8.0f;

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