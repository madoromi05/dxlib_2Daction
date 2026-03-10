#pragma once
#include "Collider/Collider.h"
#include <vector>
#include <cmath>
#include "Collider/AabbCollider.h"

class Map;

/// <summary>
/// キャラクターの移動・ジャンプ・重力・当たり判定を管理するクラス。
/// </summary>
class MovementController {
public:
    MovementController();
    void Initialize();

    void UpdateColliderPositions();
    void MoveHorizontal(Map* map, bool isRightPressed, bool isLeftPressed);
	void SetGravityDirection(int direction);    // 重力方向の切り替え
    void UpdatePhysics(Map* map);

    void ApplyBounce();
    bool IsFallingInGravityDirection() const;   //踏みけ可能か

    // 座標・状態の取得
    float GetX() const { return m_posX; }
    float GetY() const { return m_posY; }
    int GetHorizontalSpeed() const { return m_horizontalSpeed; }
    int GetVerticalSpeed() const { return m_verticalSpeed; }
    bool IsGrounded() const { return m_isGrounded; }
    bool IsFacingLeft() const { return m_isFacingLeft; }
    int GetGravityDirection() const { return m_gravityDirection; }

    // コライダー（デバッグ描画用）
    const AabbCollider& GetBodyCollider() const { return m_bodyCollider; }

private:
    void Physics(Map* map);
    bool IsCollidingWithMap(Map* map, float nextX, float nextY);

    float m_posX;
    float m_posY;

    // 移動
    int m_playerSpeed = 2;
    int m_horizontalSpeed;

    // 向き
    bool m_isFacingLeft;

    // 重力
    int m_gravityDirection;
    int m_verticalSpeed;
    bool m_isGrounded;


    // 重力パラメータ
    static constexpr int kGravityAccel = 150;
    static constexpr int kMaxFallSpeed = 100;
    int m_verticalForceDecimalPart;

    // 踏みつけ跳ね返り速度（重力と逆方向にこの速度が設定される）
    static constexpr int kBounceSpeed = 8;

    // コライダー（体のみ）
    AabbCollider m_bodyCollider;

    static constexpr float kSpriteWidth = 128.0f;
    static constexpr float kSpriteHeight = 128.0f;
};