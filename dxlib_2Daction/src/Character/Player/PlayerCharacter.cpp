#include "PlayerCharacter.h"
#include "DxLib.h"
#include "map/Map.h"

PlayerCharacter::PlayerCharacter()
    : m_isDebugMode(true)
    , m_isBouncing(false)
{}

void PlayerCharacter::Initialize(Map* map) {
    m_movement.Initialize(map);
    m_animation.Initialize();
    m_isBouncing = false;
}

void PlayerCharacter::Update(Map* map) {
    m_input.Update();
	// 重力の切り替え
    HandleGravityInput();
    m_movement.MoveHorizontal(map, m_input.IsRightPressed(), m_input.IsLeftPressed());
    m_movement.UpdatePhysics(map);
    if (m_isBouncing && m_movement.IsGrounded()) {
        m_isBouncing = false;
    }
    m_movement.UpdateColliderPositions();

    // アニメーション
    UpdateAnimationState();
    m_animation.Update();
}

void PlayerCharacter::HandleGravityInput() {
    if (m_input.IsGravityUpTriggered()) {
        m_movement.SetGravityDirection(-1);
    }
    else if (m_input.IsGravityDownTriggered()) {
        m_movement.SetGravityDirection(+1);
    }
}

void PlayerCharacter::OnStompEnemy() {
    m_movement.ApplyBounce();
    m_isBouncing = true;
}

void PlayerCharacter::UpdateAnimationState() {
    // 跳ね返り中は着地まで Bounce アニメーションを維持
    if (m_isBouncing) return;

    if (m_movement.IsGrounded()) {
        if (m_movement.GetHorizontalSpeed() != 0) {
            m_animation.ChangeState(AnimState::Run);
        }
        else {
            m_animation.ChangeState(AnimState::Idle);
        }
    }
    else {
        m_animation.ChangeState(AnimState::Falling);
    }
}

void MovementController::ApplyBounce() {
    m_verticalSpeed = -kBounceSpeed * m_gravityDirection;
    m_verticalForceDecimalPart = 0;
    m_isGrounded = false;
}

bool MovementController::IsFallingInGravityDirection() const {
    if (m_isGrounded) return false;
    return (m_verticalSpeed * m_gravityDirection) > 0;
}
void PlayerCharacter::Draw(float cameraX, float cameraY) const {
    float drawX = m_movement.GetX() - cameraX;
    float drawY = m_movement.GetY() - cameraY;

    bool isGravityUp = (m_movement.GetGravityDirection() == -1);

    // アニメーション描画（左右・上下反転対応）
    m_animation.Draw(drawX, drawY, kDrawWidth, kDrawHeight,
        m_movement.IsFacingLeft(), isGravityUp);

    // デバッグ描画
    if (m_isDebugMode) {
        // 体コライダー（緑）
        const AabbCollider& body = m_movement.GetBodyCollider();
        Vec2 bodyMin, bodyMax;
        body.GetBoundingBox(bodyMin, bodyMax);
        DrawBox(
            static_cast<int>(bodyMin.m_x - cameraX),
            static_cast<int>(bodyMin.m_y - cameraY),
            static_cast<int>(bodyMax.m_x - cameraX),
            static_cast<int>(bodyMax.m_y - cameraY),
            GetColor(0, 255, 0), FALSE
        );

        // デバッグテキスト
        DrawFormatString(0, 0, GetColor(255, 255, 255),
			_T("Grounded: %d"), m_movement.IsGrounded());
        DrawFormatString(0, 20, GetColor(255, 255, 255),
			_T("Speed Y : %d"), m_movement.GetVerticalSpeed());
        DrawFormatString(0, 40, GetColor(255, 255, 255),
			_T("Gravity : %s"), m_movement.GetGravityDirection() == 1 ? _T("DOWN") : _T("UP"));
        DrawFormatString(0, 60, GetColor(255, 255, 255),
			_T("Bounce  : %d"), m_isBouncing);
    }
}
