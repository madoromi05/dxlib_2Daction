#pragma once
#include "DxLib.h"

/// <summary>
/// プレイヤーの入力状態を毎フレーム取得・保持するクラス。
/// </summary>
class InputHandler {
public:
    InputHandler();
    void Update();

    // --- 横移動 ---
    bool IsRightPressed()  const { return m_isRightPressed; }
    bool IsLeftPressed()   const { return m_isLeftPressed; }

    // --- 重力切り替え（押した瞬間だけ true） ---
    bool IsGravityUpTriggered()   const { return m_isUpPressed && !m_prevUpPressed; }
    bool IsGravityDownTriggered() const { return m_isDownPressed && !m_prevDownPressed; }

private:
    bool m_isRightPressed;
    bool m_isLeftPressed;
    bool m_isUpPressed;
    bool m_isDownPressed;

    bool m_prevUpPressed;
    bool m_prevDownPressed;
};
