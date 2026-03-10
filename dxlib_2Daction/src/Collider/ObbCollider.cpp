#include "ObbCollider.h"
#include "DxLib.h"
#include <algorithm>

ObbCollider::ObbCollider(float halfWidth, float halfHeight, float offsetX, float offsetY)
    : m_halfWidth(halfWidth)
    , m_halfHeight(halfHeight)
    , m_offsetX(offsetX)
    , m_offsetY(offsetY)
    , m_angle(0.0f)
{}

void ObbCollider::SetWorldPosition(float posX, float posY, bool isFacingLeft,
    float spriteWidth, float angleRad) {
    float actualOffsetX = m_offsetX;
    float actualAngle = angleRad;

    // 左向きの場合:
    //   1. オフセットのXをスプライト幅基準で反転
    //   2. 回転角度を反転（鏡像）
    if (isFacingLeft) {
        actualOffsetX = spriteWidth - m_offsetX;
        actualAngle = -angleRad;
    }

    m_worldCenter = Vec2(posX + actualOffsetX, posY + m_offsetY);
    m_angle = actualAngle;

    RecalculateVertices();
}

void ObbCollider::RecalculateVertices() {
    // 中心を原点としたローカル座標の4頂点
    // 左上 → 右上 → 右下 → 左下（時計回り）
    Vec2 localVerts[4] = {
        Vec2(-m_halfWidth, -m_halfHeight),
        Vec2(m_halfWidth, -m_halfHeight),
        Vec2(m_halfWidth,  m_halfHeight),
        Vec2(-m_halfWidth,  m_halfHeight)
    };

    // 各頂点を回転してワールド座標に変換
    // 回転行列: [cos(θ) -sin(θ)] [x]   [x*cos(θ) - y*sin(θ)]
    //           [sin(θ)  cos(θ)] [y] = [x*sin(θ) + y*cos(θ)]
    for (int i = 0; i < 4; ++i) {
        m_vertices[i] = localVerts[i].Rotated(m_angle) + m_worldCenter;
    }
}

Vec2 ObbCollider::GetWorldCenter() const {
    return m_worldCenter;
}

void ObbCollider::GetBoundingBox(Vec2& outMin, Vec2& outMax) const {
    // 回転後の4頂点から、それを囲むAABBを計算する
    float minX = m_vertices[0].m_x;
    float maxX = m_vertices[0].m_x;
    float minY = m_vertices[0].m_y;
    float maxY = m_vertices[0].m_y;

    for (int i = 1; i < 4; ++i) {
        minX = (std::min)(minX, m_vertices[i].m_x);
        maxX = (std::max)(maxX, m_vertices[i].m_x);
        minY = (std::min)(minY, m_vertices[i].m_y);
        maxY = (std::max)(maxY, m_vertices[i].m_y);
    }

    outMin = Vec2(minX, minY);
    outMax = Vec2(maxX, maxY);
}

void ObbCollider::DrawDebug(unsigned int color) const {
    // 4辺を線で描画
    for (int i = 0; i < 4; ++i) {
        int next = (i + 1) % 4;
        DrawLine(
            static_cast<int>(m_vertices[i].m_x),
            static_cast<int>(m_vertices[i].m_y),
            static_cast<int>(m_vertices[next].m_x),
            static_cast<int>(m_vertices[next].m_y),
            color
        );
    }
}

std::vector<Vec2> ObbCollider::GetWorldVertices() const {
    return { m_vertices[0], m_vertices[1], m_vertices[2], m_vertices[3] };
}

std::vector<Vec2> ObbCollider::GetSeparatingAxes() const {
    // OBBの分離軸 = 各辺に垂直な法線ベクトル（正規化済み）
    // 矩形なので対辺は平行 → 2本の法線で十分
    Vec2 edge0 = m_vertices[1] - m_vertices[0];  // 上辺
    Vec2 edge1 = m_vertices[3] - m_vertices[0];  // 左辺

    return {
        edge0.Perpendicular().Normalized(),
        edge1.Perpendicular().Normalized()
    };
}