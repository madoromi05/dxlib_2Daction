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
    , m_animTimer(0)
    , m_currentFrame(0)
    , m_isDebugMode(true)
{
    // ハンドル配列を無効値で初期化
    for (int i = 0; i < kIdleFrameCount; ++i) {
        m_idleHandles[i] = -1;
    }
}

void Character::Initialize() {
    int result = LoadDivGraph(
        ResourcePath::Player::PLAYER_IMAGE,
        kIdleFrameCount,                    // 総数: 6
        kIdleFrameCount,                    // 横の数: 6
        1,                                  // 縦の数: 1
        kChipWidth,                         // 1コマの幅
        kChipHeight,                        // 1コマの高さ
        m_idleHandles                       // 保存先配列
    );

    if (result == -1) {
        std::string errorMsg = "画像の読み込みに失敗しました。\nパス: ";
        throw std::runtime_error(errorMsg);
    }

    // キャラクターの初期位置をセット
    m_playerX = 0;
    m_playerY = 100;

    m_colliders.clear();
    // [0] Body (地形判定用: 緑色で表示予定)
    m_colliders.push_back(Collider(64.0f, 120.0f, 32.0f, 8.0f));

    // [1] Attack (攻撃判定用: 赤色で表示予定。例として右側に突き出した横長の判定)
    m_colliders.push_back(Collider(60.0f, 40.0f, 96.0f, 40.0f));

    // [2] Damage (被ダメージ判定用: 青色で表示予定。Bodyより一回り小さい判定)
    m_colliders.push_back(Collider(48.0f, 100.0f, 40.0f, 18.0f));

    ResetJumpParam();
}

void Character::ResetJumpParam() {
    m_verticalSpeed = 0;
    m_verticalForce = 0;
    m_verticalForceFall = 0;
    m_verticalForceDecimalPart = 0;
    m_correctionValue = 0;
    m_isGrounded = true;
}

void Character::Update(Map* map) {
    m_animTimer++;        // タイマーを進める
    if (m_animTimer >= kAnimSpeed) {
        m_animTimer = 0;  // タイマーリセット
        m_currentFrame++; // 次のコマへ

        // 最後のコマまで行ったら最初(0)に戻す
        if (m_currentFrame >= kIdleFrameCount) {
            m_currentFrame = 0;
        }
    }

    int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

    // 横方向の移動
    Move(map, key);

    // ジャンプと重力の処理
    bool jumpBtnPress = (key & PAD_INPUT_UP) != 0;
    JumpMove(map, jumpBtnPress);
}

// Playerの入力による移動
void Character::Move(Map* map, int key) {
    float nextX = m_playerX;

    if (key & PAD_INPUT_RIGHT) {
        nextX += m_playerSpeed;
        m_horizontalSpeed = m_playerSpeed;
    }
    else if (key & PAD_INPUT_LEFT) {
        nextX -= m_playerSpeed;
        m_horizontalSpeed = -m_playerSpeed;
    }
    else {
        m_horizontalSpeed = 0;
    }

    if (map != nullptr) {
        int bodyIdx = static_cast<int>(ColliderType::Body);
        if (!m_colliders[bodyIdx].IsCollidingWithMap(map, nextX, m_playerY)) {
            m_playerX = nextX;
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
        m_verticalForce = m_verticalForceFall;
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
            }
        }
    }

    Physics(map);
}

void Character::Physics(Map* map) {
    int bodyIdx = static_cast<int>(ColliderType::Body);
    if (m_isGrounded && map != nullptr) {
        if (!m_colliders[bodyIdx].IsCollidingWithMap(map, m_playerX, m_playerY + 1.0f)) {
            m_isGrounded = false; // 足場がないので落下開始
            m_verticalForce = kVerticalFallForceData[0]; // 最低限の落下加速度をセット
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

void Character::Draw() const {
    if (m_idleHandles[m_currentFrame] != -1) {
        int oldMode = GetDrawMode();
        SetDrawMode(DX_DRAWMODE_BILINEAR);

        DrawExtendGraph(
            m_playerX,
            m_playerY,
            m_playerX + kDrawWidth,  // 定数を使う
            m_playerY + kDrawHeight, // 定数を使う
            m_idleHandles[m_currentFrame],
            TRUE
        );
        SetDrawMode(oldMode);
        if (m_isDebugMode) {
        // Body (緑)
        m_colliders[static_cast<int>(ColliderType::Body)].DrawDebug(m_playerX, m_playerY, GetColor(0, 255, 0));
        
        // Attack (赤)
        m_colliders[static_cast<int>(ColliderType::Attack)].DrawDebug(m_playerX, m_playerY, GetColor(255, 0, 0));
        
        // Damage (青)
        m_colliders[static_cast<int>(ColliderType::Damage)].DrawDebug(m_playerX, m_playerY, GetColor(0, 0, 255));

        DrawFormatString(0, 0, GetColor(255, 255, 255), "Grounded: %d", m_isGrounded);
        DrawFormatString(0, 20, GetColor(255, 255, 255), "Speed Y : %d", m_verticalSpeed);
        DrawFormatString(0, 40, GetColor(255, 255, 255), "Player Y: %.1f", m_playerY);
        }
    }
}