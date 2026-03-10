#pragma once
#pragma once
#include "ICollider.h"

/// <summary>
/// OBB（有向境界矩形）コライダー。
/// 回転角度を持つ矩形で、攻撃判定のように角度がつく場面で使用する。
/// 
/// 分離軸定理（SAT: Separating Axis Theorem）で衝突を判定する:
///   1. 両OBBの各辺に垂直な法線を分離軸候補とする（最大4本）
///   2. 両OBBの全頂点を各軸に射影（内積で計算）
///   3. 全ての軸で射影区間が重なっていれば衝突
///   4. 1本でも重ならない軸があれば非衝突（分離軸が存在）
/// </summary>
class ObbCollider : public ICollider {
public:
    /// <param name="halfWidth">中心からX方向の半分の幅</param>
    /// <param name="halfHeight">中心からY方向の半分の高さ</param>
    /// <param name="offsetX">画像左上から判定中心までのXオフセット（右向き基準）</param>
    /// <param name="offsetY">画像左上から判定中心までのYオフセット</param>
    ObbCollider(float halfWidth, float halfHeight, float offsetX, float offsetY);

    /// <summary>
    /// キャラクターのワールド座標・向き・回転角度を設定する。
    /// 左向きの場合、オフセットのXが反転され、回転角度も反転される。
    /// </summary>
    /// <param name="posX">キャラクターのワールドX座標</param>
    /// <param name="posY">キャラクターのワールドY座標</param>
    /// <param name="isFacingLeft">左向きかどうか</param>
    /// <param name="spriteWidth">スプライトの幅（反転計算用）</param>
    /// <param name="angleRad">回転角度（ラジアン）。右向き基準で正が時計回り</param>
    void SetWorldPosition(float posX, float posY, bool isFacingLeft,
        float spriteWidth, float angleRad = 0.0f);

    // --- ICollider 実装 ---
    ShapeType GetShapeType() const override { return ShapeType::Obb; }
    Vec2 GetWorldCenter() const override;
    void GetBoundingBox(Vec2& outMin, Vec2& outMax) const override;
    void DrawDebug(unsigned int color) const override;
    std::vector<Vec2> GetWorldVertices() const override;
    std::vector<Vec2> GetSeparatingAxes() const override;

private:
    /// <summary>
    /// 回転後の4頂点を再計算する内部関数
    /// </summary>
    void RecalculateVertices();

    float m_halfWidth;
    float m_halfHeight;
    float m_offsetX;        // 右向き基準の中心オフセット
    float m_offsetY;

    Vec2 m_worldCenter;     // ワールド座標での中心
    float m_angle;          // 現在の回転角度（ラジアン）

    // 回転後の4頂点（ワールド座標）
    Vec2 m_vertices[4];
};