#pragma once
#include "ICollider.h"


/// <summary>
/// AABB（軸平行境界矩形）コライダー。
/// 回転しない矩形で、地形との判定やキャラクターの体判定に使用する。
/// 
/// オフセットはキャラクター画像の左上を原点とした相対座標で、
/// SetWorldPosition() でキャラクターのワールド座標を設定すると
/// ワールド座標のAABBが計算される。
/// </summary>
class AabbCollider : public ICollider {
public:
    /// <param name="width">矩形の幅</param>
    /// <param name="height">矩形の高さ</param>
    /// <param name="offsetX">画像左上からのXオフセット</param>
    /// <param name="offsetY">画像左上からのYオフセット</param>
    AabbCollider(float width, float height, float offsetX, float offsetY);

    /// <summary>
    /// キャラクターのワールド座標と向きを設定する。
    /// 左向きの場合、オフセットのX座標がピボットを基準に反転される。
    /// </summary>
    /// <param name="posX">キャラクターのワールドX座標</param>
    /// <param name="posY">キャラクターのワールドY座標</param>
    /// <param name="isFacingLeft">左向きかどうか</param>
    /// <param name="spriteWidth">スプライトの幅（反転計算用）</param>
    void SetWorldPosition(float posX, float posY, bool isFacingLeft, float spriteWidth);

    // ICollider 実装
    ShapeType GetShapeType() const override { return ShapeType::Aabb; }
    Vec2 GetWorldCenter() const override;
    void GetBoundingBox(Vec2& outMin, Vec2& outMax) const override;
    void DrawDebug(unsigned int color) const override;
    std::vector<Vec2> GetWorldVertices() const override;
    std::vector<Vec2> GetSeparatingAxes() const override;

    // AABB固有のアクセサ
    float GetLeft() const { return m_worldMin.m_x; }
    float GetRight() const { return m_worldMax.m_x; }
    float GetTop() const { return m_worldMin.m_y; }
    float GetBottom() const { return m_worldMax.m_y; }

private:
    float m_width;
    float m_height;
    float m_offsetX;    // 画像左上からのXオフセット（右向き基準）
    float m_offsetY;    // 画像左上からのYオフセット

    Vec2 m_worldMin;    // ワールド座標での左上
    Vec2 m_worldMax;    // ワールド座標での右下
};