#include "Character.h"
#include "ResorcePath.h"
#include "DxLib.h"
#include "map/Map.h"
#include "Collider/Collider.h"
#include <stdexcept>
#include <string>
#include <cmath>
#include <vector>

Character::Character()
    : m_playerX(640)
    , m_playerY(480)
    , m_verticalPositionOrigin(0)
    , m_verticalSpeed(0)
    , m_verticalForce(0)
    , m_verticalForceFall(0)
    , m_verticalForceDecimalPart(0)
    , m_correctionValue(0)
    , m_horizontalSpeed(0)
    , m_jumpBtnPrevPress(false)
    , m_isGrounded(false)
    , m_isDebugMode(true)
	, m_isJumping(false)
    , m_currentAnimation(nullptr)
    , m_currentState(AnimState::Idle)
	, m_isFacingLeft(false)
{}

void Character::Initialize() {
    int idleIdx = static_cast<int>(AnimState::Idle);
    m_animations[idleIdx].Load(ResourcePath::Player::PLAYER_IDLE, 6, 6, 1, 128, 128);
    m_animations[idleIdx].SetAnimSpeed(8);
    m_animations[idleIdx].SetLoop(true);

	// ジャンプの上昇アニメーション
    int jumpUpIdx = static_cast<int>(AnimState::JumpUp);
    m_animations[jumpUpIdx].Load(ResourcePath::Player::PLAYER_JUMP, 6, 6, 1, 128, 128);
    m_animations[jumpUpIdx].SetAnimSpeed(5);
    m_animations[jumpUpIdx].SetLoop(false);

    // 落下アニメーション読み込み
    int jumpDownIdx = static_cast<int>(AnimState::JumpDown);
    m_animations[jumpDownIdx].Load(ResourcePath::Player::PLAYER_DOWNJUMP, 6, 6, 1, 128, 128);
    m_animations[jumpDownIdx].SetAnimSpeed(5);
    m_animations[jumpDownIdx].SetLoop(false);

	// 走りアニメーション読み込み
    int runIdx = static_cast<int>(AnimState::Run);
    m_animations[runIdx].Load(ResourcePath::Player::PLAYER_RUN, 8, 8, 1, 128, 128);
    m_animations[runIdx].SetAnimSpeed(10);
    m_animations[runIdx].SetLoop(true);

    // 初期状態をIdleにセット
    m_currentAnimation = &m_animations[idleIdx];
    m_currentState = AnimState::Idle;

    // キャラクターの初期位置をセット
    m_playerX = 0;
    m_playerY = 475;

    m_isFacingLeft = false;

    m_colliders.clear();
    // 地形判定用: 緑色
    m_colliders.push_back(Collider(60.0f, 90.0f, 34.0f, 40.0f));

    // 攻撃判定用: 赤色
    m_colliders.push_back(Collider(60.0f, 40.0f, 96.0f, 40.0f));

    // 被ダメージ判定用: 青色
    m_colliders.push_back(Collider(48.0f, 70.0f, 40.0f, 48.0f));

    ResetJumpParam();
}

void Character::ChangeAnimation(AnimState newState) {
    if (m_currentState == newState) return;

    m_currentState = newState;
    m_currentAnimation = &m_animations[static_cast<int>(newState)];
    m_currentAnimation->Reset();
}

void Character::ResetJumpParam() {
    m_verticalSpeed = 0;
    m_verticalForce = 0;
    m_verticalForceFall = 0;
    m_verticalForceDecimalPart = 0;
    m_correctionValue = 0;
    m_isGrounded = true;
	m_isJumping = false;
}

void Character::Update(Map* map) {
    m_idleAnimation.Update();

    int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

    // 横方向の移動
    Move(map, key);

    // ジャンプと重力の処理
    bool jumpBtnPress = (key & PAD_INPUT_UP) != 0;
    JumpMove(map, jumpBtnPress);

    if (m_isGrounded) {
        if (m_horizontalSpeed != 0) {
            ChangeAnimation(AnimState::Run);      // 地面にいて速度があるなら「走り」
        } else {
            ChangeAnimation(AnimState::Idle);     // 止まっているなら「待機」
        }
    }
    else {
        if (m_verticalSpeed < 0) {
            ChangeAnimation(AnimState::JumpUp);   // 上に飛んでいるなら「上昇」
        }else {
            ChangeAnimation(AnimState::JumpDown); // 落ちているなら「落下」
        }
    }
    // 現在選択されているアニメーションだけを更新する
    if (m_currentAnimation != nullptr) {
        m_currentAnimation->Update();
    }
}

void Character::Move(Map* map, int key) {
    float nextX = m_playerX;

    if (key & PAD_INPUT_RIGHT) {
        nextX += m_playerSpeed;
        m_horizontalSpeed = m_playerSpeed;
        m_isFacingLeft = false;
    }
    else if (key & PAD_INPUT_LEFT) {
        nextX -= m_playerSpeed;
        m_horizontalSpeed = -m_playerSpeed;
        m_isFacingLeft = true;
    }
    else {
        m_horizontalSpeed = 0;
    }

    if (map != nullptr) {
        int bodyIdx = static_cast<int>(ColliderType::Body);
        if (!m_colliders[bodyIdx].IsCollidingWithMap(map, nextX, m_playerY)) {
            m_playerX = nextX;
        }
        else {
            // 進めない場合、段差・スロープを登れるか試す
            float stepHeight = static_cast<float>(m_playerSpeed) + 2.0f;
            bool canStepUp = false;

            // 2ピクセルずつ上に持ち上げて、ぶつからない高さがあるか探す
            for (float s = 2.0f; s <= stepHeight; s += 2.0f) {
                if (!m_colliders[bodyIdx].IsCollidingWithMap(map, nextX, m_playerY - s)) {
                    m_playerX = nextX;    // 横に進む
                    m_playerY -= s;       // 坂の分だけ上に押し上げる
                    canStepUp = true;
                    break;
                }
            }

            // 登れない壁だった場合は、当たる直前まで進んで止まる
            if (!canStepUp) {
                float step = (nextX > m_playerX) ? 1.0f : -1.0f;
                while (!m_colliders[bodyIdx].IsCollidingWithMap(map, m_playerX + step, m_playerY)) {
                    m_playerX += step;
                }
            }
        }
    }
    else {
        m_playerX = nextX;
    }
}

void Character::JumpMove(Map* map,bool jumpBtnPress)
{
    JumpCheck(jumpBtnPress);
    MoveProcess(map,jumpBtnPress);
    m_jumpBtnPrevPress = jumpBtnPress;
}

void Character::JumpCheck(bool jumpBtnPress) {
	if (jumpBtnPress == false) return;        // 今のフレームでジャンプボタンが押されていない
	if (m_jumpBtnPrevPress == true) return;   // 前のフレームでジャンプボタンが押されていた
    
    // 地面上にいる状態？
    if (m_isGrounded)
    {
        // ジャンプ開始準備
        PreparingJump();
    }
}

void Character::MoveProcess(Map* map, bool jumpBtnPress){
    // 速度が0かプラスなら画面下へ進んでいるものとして落下状態の加速度に切り替える
    if (m_verticalSpeed >= 0)
    {
        m_verticalForce = m_isJumping ? m_verticalForceFall : kNormalGravity;
    }
    else
    {
        // Aボタンが離された&上昇中？
        if (jumpBtnPress == false && m_jumpBtnPrevPress == true)
        {
            if (m_verticalPositionOrigin - static_cast<int>(m_playerY) >= 1)
            {
                // 落下状態の加速度値に切り替える
                m_verticalForce = m_verticalForceFall;
                m_verticalSpeed /= 2;
                m_verticalForceDecimalPart = 0;
            }
        }
    }

    Physics(map);
}

void Character::Physics(Map* map) {
    int bodyIdx = static_cast<int>(ColliderType::Body);
    if (m_isGrounded && map != nullptr) {
        // 足場から落ちた場合
        if (!m_colliders[bodyIdx].IsCollidingWithMap(map, m_playerX, m_playerY + 1.0f)) {
            m_isGrounded = false;
			m_verticalForce = kNormalGravity;   // 落下状態の加速度をセット
        }
    }

    // 累積計算での補正値っぽい
    int cy = 0;
    m_correctionValue += m_verticalForceDecimalPart;
    if (m_correctionValue >= 256) {
        m_correctionValue -= 256;
        cy = 1;
    }

    // 現在位置に速度を加算 (累積計算での補正値も加算)
    float nextY = m_playerY + m_verticalSpeed + cy;

    // 加速度の固定少数点部への加算
    // 1バイトをオーバーフローしたら、速度が加算される。その時、加速度の整数部は0に戻される
    m_verticalForceDecimalPart += m_verticalForce;
    while (m_verticalForceDecimalPart >= 256) {
        m_verticalForceDecimalPart -= 256;
        m_verticalSpeed++;
    }

    // 速度の上限チェック
    if (m_verticalSpeed >= kDownSpeedLimit)
    {
        if (m_verticalForceDecimalPart >= 0x80)
        {
            m_verticalSpeed = kDownSpeedLimit;
            m_verticalForceDecimalPart = 0x00;
        }
    }

    if (map != nullptr) {
        if (!m_colliders[bodyIdx].IsCollidingWithMap(map, m_playerX, nextY)) {
            m_playerY = nextY;
        }
        else {
            if (m_verticalSpeed >= 0) {
                while (!m_colliders[bodyIdx].IsCollidingWithMap(map, m_playerX, m_playerY + 1.0f)) {
                    m_playerY += 1.0f;
                }
                ResetJumpParam();
            }
        }
    }
    else {
        m_playerY = nextY;
    }
}

void Character::PreparingJump() {
    m_verticalForceDecimalPart = 0;
    m_verticalPositionOrigin = static_cast<int>(m_playerY);
    m_isGrounded = false; // Jumping状態へ
	m_isJumping  = true;

    int idx = 0;
    int absSpeed = std::abs(m_horizontalSpeed);

    // 慣性に応じた軌道（ジャンプ力の分岐）
    if (absSpeed >= 5) idx++;
    if (absSpeed >= 4) idx++;
    if (absSpeed >= 3) idx++;
    if (absSpeed >= 1) idx++;

    m_verticalForce = kVerticalForceDecimalPartData[idx];
    m_verticalForceFall = kVerticalFallForceData[idx];
    m_verticalForceDecimalPart = kInitialVerticalForceData[idx];
    m_verticalSpeed = kInitialVerticalSpeedData[idx];
}

void Character::Draw(float cameraX, float cameraY) const {
    float drawX = m_playerX - cameraX;
    float drawY = m_playerY - cameraY;

    if (m_currentAnimation != nullptr) {
        m_currentAnimation->Draw(drawX, drawY, kDrawWidth, kDrawHeight, m_isFacingLeft);
    }

    if (m_isDebugMode) {
        m_colliders[static_cast<int>(ColliderType::Body)].DrawDebug(drawX, drawY, GetColor(0, 255, 0));
        m_colliders[static_cast<int>(ColliderType::Attack)].DrawDebug(drawX, drawY, GetColor(255, 0, 0));
        m_colliders[static_cast<int>(ColliderType::Damage)].DrawDebug(drawX, drawY, GetColor(0, 0, 255));

        DrawFormatString(0, 0, GetColor(255, 255, 255), "Grounded: %d", m_isGrounded);
        DrawFormatString(0, 20, GetColor(255, 255, 255), "Speed Y : %d", m_verticalSpeed);
        DrawFormatString(0, 40, GetColor(255, 255, 255), "Player Y: %.1f", m_playerY);
    }
}