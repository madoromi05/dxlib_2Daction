#pragma once
#include "Vecter2.h"
#include <vector>

/// <summary>
/// コライダーの形状タイプ
/// </summary>
enum class ShapeType {
    Aabb,       // 軸平行矩形
    Obb,        // 回転対応矩形
    Circle      // 円形
};

/// <summary>
/// 全コライダーの基底インターフェース。
/// </summary>
class ICollider {
public:
    virtual ~ICollider() = default;

    /// <summary>
    /// コライダーの形状タイプを返す
    /// ダブルディスパッチで判定関数を選択するために使用する
    /// </summary>
    virtual ShapeType GetShapeType() const = 0;

    /// <summary>
    /// ワールド座標でのコライダー中心を返す
    /// </summary>
    virtual Vec2 GetWorldCenter() const = 0;

    /// <summary>
    /// このコライダーを完全に囲むAABBを返す（ブロードフェーズ用）
    /// 返り値: [0]=左上, [1]=右下
    /// </summary>
    virtual void GetBoundingBox(Vec2& outMin, Vec2& outMax) const = 0;

    /// <summary>
    /// デバッ用当たり判定描画
    /// </summary>
    virtual void DrawDebug(unsigned int color) const = 0;

    /// <summary>
    /// ワールド座標での頂点リストを返す（SAT判定に使用）
    /// 円形の場合は空を返す
    /// </summary>
    virtual std::vector<Vec2> GetWorldVertices() const { return {}; }

    /// <summary>
    /// SAT判定で使用する分離軸のリストを返す
    /// 円形の場合は空を返す（円は動的に軸を決めるため）
    /// </summary>
    virtual std::vector<Vec2> GetSeparatingAxes() const { return {}; }
};