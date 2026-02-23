#include "Character.h"
#include "ResorcePath.h"
#include "DxLib.h"
#include "map/Map.h"

Character::Character()
    : m_playerX(640)
    , m_playerY(480)
	, m_isGrounded(false)
    , m_animTimer(0)
    , m_currentFrame(0)
    , m_nextX(0)
    , m_nextY(0)
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
        kChipWidth,                         // 1コマの幅 (要確認)
        kChipHeight,                        // 1コマの高さ (要確認)
        m_idleHandles                       // 保存先配列
    );

    if (result == -1) {
        std::string errorMsg = "画像の読み込みに失敗しました。\nパス: ";
        throw std::runtime_error(errorMsg);
    }

    // キャラクターの初期位置をセット
    m_playerX = 0;
    m_playerY = 200;
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
    m_nextY += kGravity;

    m_nextX = m_playerX;
    m_nextY = m_playerY;


    int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    Move(map, key);
    Jump(key);
    Gravity(map);
}

// Playerの入力による移動
void Character::Move(Map* map, int key) {
    m_nextX = m_playerX;
    m_nextY = m_playerY;

    if (key & PAD_INPUT_RIGHT) m_nextX += m_playerSpeed;
    if (key & PAD_INPUT_LEFT)  m_nextX -= m_playerSpeed;

    if (map != nullptr) {
        if (!map->IsWall(m_nextX, m_nextY)) {
            m_playerX = m_nextX;
            m_playerY = m_nextY;
        }
    }
    else {
        m_playerX = m_nextX;
        m_playerY = m_nextY;
    }
}

void Character::Jump(int key) {
    if (m_isGrounded && (key & PAD_INPUT_UP)) {
        m_velocityY = kJumpPower; // 上方向へ初速を与える
        m_isGrounded = false;     // 空中判定にする
    }
}

void Character::Gravity(Map* map) {
    m_velocityY += kGravity;
    float nextY = m_playerY + m_velocityY;

    if (map != nullptr) {
        if (!map->IsWall(m_playerX, nextY)) {
            m_playerY = nextY;
            m_isGrounded = false;
        }
        else {
            if (m_velocityY > 0) {
                m_isGrounded = true;
            }
            m_velocityY = 0.0f;
        }
    }
    else {
        m_playerY = nextY;
    }
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
    }
}