#pragma once
#include "SpriteAnimation.h"

/// <summary>
/// アニメーションの状態定義
/// </summary>
enum class AnimState {
    Idle,       // 待機
    Run,        // 走り
    Falling,    // 落下中（重力方向に落ちている）
    Bounce,     // 踏みつけ後の跳ね返り中
    Count       // 状態の総数（最後に配置）
};

/// <summary>
/// キャラクターのアニメーション読み込み・状態遷移・更新・描画を管理するクラス。
/// </summary>
class AnimationController {
public:
    AnimationController();
    void Initialize();
    void Update();

    /// 指定座標にアニメーションを描画する
    void Draw(float x, float y, int drawWidth, int drawHeight,
        bool isFacingLeft, bool isGravityUp) const;

    /// アニメーション状態を切り替える（同じ状態なら何もしない）
    void ChangeState(AnimState newState);

    /// 現在のアニメーションが最後まで再生されたか
    bool IsCurrentFinished() const;

    /// 現在のアニメーション状態を取得する
    AnimState GetCurrentState() const { return m_currentState; }

private:
    SpriteAnimation m_animations[static_cast<int>(AnimState::Count)];
    SpriteAnimation* m_currentAnimation;
    AnimState m_currentState;
};