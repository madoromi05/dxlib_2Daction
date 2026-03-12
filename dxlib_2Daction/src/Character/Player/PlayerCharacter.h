#pragma once
#include "DrawInterface/IDrawable.h"
#include "InputHandler.h"
#include "MovementController.h"
#include "../Animertion/AnimationController.h"

class Map;

/// <summary>
/// プレイヤーキャラクター（重力操作版）
///
/// 操作:
///   左右キー: 横移動
///   上キー:   重力を上向きに切り替え（天井に落ちる）
///   下キー:   重力を下向きに切り替え（床に落ちる）
/// </summary>
class PlayerCharacter : public IDrawable {
public:
    PlayerCharacter();
    void Initialize(Map* map);
    void Update(class Map* map);
    void Draw(float cameraX, float cameraY) const override;

    int GetX() const { return static_cast<int>(m_movement.GetX()); }
    int GetY() const { return static_cast<int>(m_movement.GetY()); }

    MovementController& GetMovement() { return m_movement; }
    const MovementController& GetMovement() const { return m_movement; }

	// 敵を踏んだときの処理
    void OnStompEnemy();
    bool IsBouncing() const { return m_isBouncing; }

private:
    void UpdateAnimationState();
    void HandleGravityInput();

    InputHandler m_input;
    MovementController m_movement;
    AnimationController m_animation;

    bool m_isDebugMode;
    bool m_isBouncing;

    // キャラクターの描画サイズ
    static constexpr int kDrawWidth = 128;
    static constexpr int kDrawHeight = 128;
};
