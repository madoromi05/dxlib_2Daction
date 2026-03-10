#pragma once
#include "ICollider.h"
#include "DxLib.h"

/// <summary>
/// 円形コライダー。
/// 球体の敵との衝突判定に使用する。
/// </summary>
class CircleCollider : public ICollider {
public:
    /// <param name="radius">円の半径</param>
    /// <param name="offsetX">画像左上から円の中心までのXオフセット（右向き基準）</param>
    /// <param name="offsetY">画像左上から円の中心までのYオフセット</param>
	/// <param name="isFacingLeft">左を向いているか（trueならoffsetXを反転）</param>
    CircleCollider(float radius, float offsetX, float offsetY);

    /// <summary>
    /// ワールド座標と向きを設定する
    /// </summary>
    void SetWorldPosition(float posX, float posY, bool isFacingLeft, float spriteWidth);

    // ICollider 実装
    ShapeType GetShapeType() const override { return ShapeType::Circle; }
    Vec2 GetWorldCenter() const override;
    void GetBoundingBox(Vec2& outMin, Vec2& outMax) const override;
    void DrawDebug(unsigned int color) const override;
    float GetRadius() const { return m_radius; }

private:
    float m_radius;
    float m_offsetX;
    float m_offsetY;

    Vec2 m_worldCenter;
};