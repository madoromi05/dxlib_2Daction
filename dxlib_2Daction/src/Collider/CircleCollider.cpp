#include "CircleCollider.h"

CircleCollider::CircleCollider(float radius, float offsetX, float offsetY)
    : m_radius(radius)
    , m_offsetX(offsetX)
    , m_offsetY(offsetY)
{}

void CircleCollider::SetWorldPosition(float posX, float posY, bool isFacingLeft, float spriteWidth) {
    float actualOffsetX = m_offsetX;

    if (isFacingLeft) {
        actualOffsetX = spriteWidth - m_offsetX;
    }

    m_worldCenter = Vec2(posX + actualOffsetX, posY + m_offsetY);
}

Vec2 CircleCollider::GetWorldCenter() const {
    return m_worldCenter;
}

void CircleCollider::GetBoundingBox(Vec2& outMin, Vec2& outMax) const {
    // 円を囲むAABB
    outMin = Vec2(m_worldCenter.m_x - m_radius, m_worldCenter.m_y - m_radius);
    outMax = Vec2(m_worldCenter.m_x + m_radius, m_worldCenter.m_y + m_radius);
}

void CircleCollider::DrawDebug(unsigned int color) const {
    DrawCircle(
        static_cast<int>(m_worldCenter.m_x),
        static_cast<int>(m_worldCenter.m_y),
        static_cast<int>(m_radius),
        color, FALSE
    );
}