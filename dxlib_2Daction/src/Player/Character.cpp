#include "Character.h"
#include <stdexcept>
#include <string>
#include "DxLib.h"
#include "map/Map.h"

Character::Character()
    : m_playerX(320)
    , m_playerY(240)
    , m_animTimer(0)
    , m_currentFrame(0)
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
        // errorMsg += ResourcePath::Player::PLAYER_IMAGE; // string型なら結合可能
        throw std::runtime_error(errorMsg);
    }

    // キャラクターの初期位置をセット
    m_playerX = 0;
    m_playerY = 0;
}

void Character::Update(Map* map) {
    m_animTimer++; // タイマーを進める
    if (m_animTimer >= kAnimSpeed) {
        m_animTimer = 0; // タイマーリセット
        m_currentFrame++; // 次のコマへ

        // 最後のコマまで行ったら最初(0)に戻す
        if (m_currentFrame >= kIdleFrameCount) {
            m_currentFrame = 0;
        }
    }

    // 移動処理
    int nextX = static_cast<int>(m_playerX);
    int nextY = static_cast<int>(m_playerY);
	int speed = 3;

    int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

    if (key & PAD_INPUT_UP)    nextY -= speed;
    if (key & PAD_INPUT_DOWN)  nextY += speed;
    if (key & PAD_INPUT_RIGHT) nextX += speed;
    if (key & PAD_INPUT_LEFT)  nextX -= speed;

    if (map != nullptr) {
        if (!map->IsWall(nextX, nextY)) {
            m_playerX = nextX;
            m_playerY = nextY;
        }
    }
    else {
        m_playerX = nextX;
        m_playerY = nextY;
    }
}

void Character::Draw() const {
    if (m_idleHandles[m_currentFrame] != -1) {
        DrawGraph(
            static_cast<int>(m_playerX),
            static_cast<int>(m_playerY),
            m_idleHandles[m_currentFrame],
            TRUE
        );
    }
}