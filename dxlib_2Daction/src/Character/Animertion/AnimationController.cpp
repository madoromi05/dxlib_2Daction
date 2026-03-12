#include "AnimationController.h"
#include "ResourcePath.h"

AnimationController::AnimationController()
    : m_currentAnimation(nullptr)
    , m_currentState(AnimState::Idle)
{}

void AnimationController::Initialize() {
    // 待機アニメーション
    int idleIdx = static_cast<int>(AnimState::Idle);
    m_animations[idleIdx].Load(ResourcePath::Player::kPlayerIdle, 6, 6, 1, 128, 128);
    m_animations[idleIdx].SetAnimSpeed(8);
    m_animations[idleIdx].SetLoop(true);

    // 走り
    int runIdx = static_cast<int>(AnimState::Run);
    m_animations[runIdx].Load(ResourcePath::Player::kPlayerRun, 8, 8, 1, 128, 128);
    m_animations[runIdx].SetAnimSpeed(10);
    m_animations[runIdx].SetLoop(true);

    // 落下
    int fallIdx = static_cast<int>(AnimState::Falling);
    m_animations[fallIdx].Load(ResourcePath::Player::kPlayerDownJump, 6, 6, 1, 128, 128);
    m_animations[fallIdx].SetAnimSpeed(10);
    m_animations[fallIdx].SetLoop(false);

    // 跳ね返り
    int bounceIdx = static_cast<int>(AnimState::Bounce);
    m_animations[bounceIdx].Load(ResourcePath::Player::kPlayerJump, 6, 6, 1, 128, 128);
    m_animations[bounceIdx].SetAnimSpeed(10);
    m_animations[bounceIdx].SetLoop(false);

    // 初期状態をIdleにセット
    m_currentAnimation = &m_animations[idleIdx];
    m_currentState = AnimState::Idle;
}

void AnimationController::Update() {
    if (m_currentAnimation != nullptr) {
        m_currentAnimation->Update();
    }
}

void AnimationController::Draw(float x, float y, int drawWidth, int drawHeight,
    bool isFacingLeft, bool isGravityUp) const {
    if (m_currentAnimation == nullptr) return;

    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);

    int leftX = ix;
    int rightX = ix + drawWidth;
    int topY = iy;
    int bottomY = iy + drawHeight;

    if (isFacingLeft && isGravityUp) {
        // 左右 + 上下反転
        m_currentAnimation->DrawModified(
            rightX, bottomY,
            leftX, bottomY,
            leftX, topY,
            rightX, topY
        );
    }
    else if (isFacingLeft) {
        // 左右反転のみ
        m_currentAnimation->DrawModified(
            rightX, topY,
            leftX, topY,
            leftX, bottomY,
            rightX, bottomY
        );
    }
    else if (isGravityUp) {
        // 上下反転のみ
        m_currentAnimation->DrawModified(
            leftX, bottomY,
            rightX, bottomY,
            rightX, topY,
            leftX, topY
        );
    }
    else {
        // 通常描画
        m_currentAnimation->Draw(x, y, drawWidth, drawHeight, false);
    }
}

void AnimationController::ChangeState(AnimState newState) {
    if (m_currentState == newState) return;

    m_currentState = newState;
    m_currentAnimation = &m_animations[static_cast<int>(newState)];
    m_currentAnimation->Reset();
}

bool AnimationController::IsCurrentFinished() const {
    if (m_currentAnimation == nullptr) return false;
    return m_currentAnimation->IsFinished();
}
