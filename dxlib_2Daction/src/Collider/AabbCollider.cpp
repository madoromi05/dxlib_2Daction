#include "AabbCollider.h"
#include "DxLib.h"

AabbCollider::AabbCollider(float width, float height, float offsetX, float offsetY)
    : m_width(width)
    , m_height(height)
    , m_offsetX(offsetX)
    , m_offsetY(offsetY)
{
}

void AabbCollider::SetWorldPosition(float posX, float posY, bool isFacingLeft, float spriteWidth) {
    float actualOffsetX = m_offsetX;

    // 左向きの場合: スプライト幅を基準にオフセットのX座標を反転
    // 右向き時のオフセット位置を、スプライト中心を対称軸として鏡像にする
    if (isFacingLeft) {
        actualOffsetX = spriteWidth - m_offsetX - m_width;
    }

    m_worldMin = Vec2(posX + actualOffsetX, posY + m_offsetY);
    m_worldMax = Vec2(m_worldMin.m_x + m_width, m_worldMin.m_y + m_height);
}

Vec2 AabbCollider::GetWorldCenter() const {
    return Vec2(
        (m_worldMin.m_x + m_worldMax.m_x) * 0.5f,
        (m_worldMin.m_y + m_worldMax.m_y) * 0.5f
    );
}

void AabbCollider::GetBoundingBox(Vec2& outMin, Vec2& outMax) const {
    outMin = m_worldMin;
    outMax = m_worldMax;
}

void AabbCollider::DrawDebug(unsigned int color) const {
    DrawBox(
        static_cast<int>(m_worldMin.m_x),
        static_cast<int>(m_worldMin.m_y),
        static_cast<int>(m_worldMax.m_x),
        static_cast<int>(m_worldMax.m_y),
        color, FALSE
    );
}

std::vector<Vec2> AabbCollider::GetWorldVertices() const {
    // 矩形の4頂点を返す（時計回り）
    return {
        m_worldMin,
        Vec2(m_worldMax.m_x, m_worldMin.m_y),
        m_worldMax,
        Vec2(m_worldMin.m_x, m_worldMax.m_y)
    };
}

std::vector<Vec2> AabbCollider::GetSeparatingAxes() const {
    // AABBの分離軸はX軸とY軸の2本のみ
    return {
        Vec2(1.0f, 0.0f),
        Vec2(0.0f, 1.0f)
    };
}