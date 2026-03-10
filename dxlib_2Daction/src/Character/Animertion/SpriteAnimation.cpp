#include "SpriteAnimation.h"
#include "DxLib.h"
#include <stdexcept>

SpriteAnimation::SpriteAnimation()
    : m_animTimer(0)
    , m_currentFrame(0)
    , m_animSpeed(0)
    , m_frameCount(0)
    , m_isLoop(true)
    , m_isFinished(false)
{}

SpriteAnimation::~SpriteAnimation() {
    for (int handle : m_handles) {
        if (handle != -1) {
            DeleteGraph(handle);
        }
    }
}

void SpriteAnimation::Load(const tstring& filePath,  int totalFrameCount, int splitX, int splitY, int frameWidth, int frameHeight) {
    m_frameCount = totalFrameCount;
    m_handles.resize(totalFrameCount, -1);

    int result = LoadDivGraph(
        filePath.c_str(),
        totalFrameCount,
        splitX,
        splitY,
        frameWidth,
        frameHeight,
        m_handles.data()
    );

    if (result == -1) {
        throw std::runtime_error("画像の読み込みに失敗しました。\nパス: ");
    }
}

void SpriteAnimation::Update() {
    if (m_frameCount <= 0 || m_isFinished) return;

    m_animTimer++;
    if (m_animTimer >= m_animSpeed) {
        m_animTimer = 0;
        m_currentFrame++;

        // 最後のコマまで行った時の処理(ループするかどうか)
        if (m_currentFrame >= m_frameCount) {
            if (m_isLoop) {
                m_currentFrame = 0;
            }
            else {
                m_currentFrame = m_frameCount - 1;
                m_isFinished = true;
            }
        }
    }
}

void SpriteAnimation::Draw(float x, float y, int drawWidth, int drawHeight, bool isTurn) const {
    if (m_frameCount > 0 && m_handles[m_currentFrame] != -1) {
        int oldMode = GetDrawMode();
        SetDrawMode(DX_DRAWMODE_BILINEAR);

        int ix = static_cast<int>(x);
        int iy = static_cast<int>(y);

        // 左向きの場合
        if (isTurn) {
            DrawModiGraph(
                ix + drawWidth, iy,               // 左上座標
                ix, iy,                           // 右上座標
                ix, iy + drawHeight,              // 右下座標
                ix + drawWidth, iy + drawHeight,  // 左下座標
                m_handles[m_currentFrame], TRUE
            );
        }
        else {
            // 右向きの場合
            DrawExtendGraph(
                ix,
                iy,
                ix + drawWidth,
                iy + drawHeight,
                m_handles[m_currentFrame],
                TRUE
            );
        }
        SetDrawMode(oldMode);
    }
}

void SpriteAnimation::DrawModified(int x1, int y1, int x2, int y2,
    int x3, int y3, int x4, int y4) const {
    if (m_frameCount > 0 && m_handles[m_currentFrame] != -1) {
        int oldMode = GetDrawMode();
        SetDrawMode(DX_DRAWMODE_BILINEAR);
        DrawModiGraph(
            x1, y1,
            x2, y2,
            x3, y3,
            x4, y4,
            m_handles[m_currentFrame], TRUE
        );

        SetDrawMode(oldMode);
    }
}

void SpriteAnimation::SetAnimSpeed(int speed) {
    m_animSpeed = speed;
}

void SpriteAnimation::Reset() {
    m_animTimer = 0;
    m_currentFrame = 0;
    m_isFinished = false;
}

void SpriteAnimation::SetLoop(bool isLoop) {
    m_isLoop = isLoop;
}

bool SpriteAnimation::IsFinished() const {
    return m_isFinished;
}
