#pragma once
#include "ICollider.h"
#include "AabbCollider.h"
#include "ObbCollider.h"
#include "CircleCollider.h"

/// <summary>
/// 衝突判定ヘルパー（静的クラス）。
/// </summary>
class CollisionHelper {
public:
    CollisionHelper() = delete;

    /// <summary>
    /// 任意の2つのコライダーの衝突を判定する。
    /// ShapeType に基づいて適切な判定関数にディスパッチする。
    /// </summary>
    static bool CheckCollision(const ICollider* a, const ICollider* b);

private:
    // ===== 各組み合わせの判定関数 =====

    /// AABB vs AABB
    static bool CheckAabbVsAabb(const AabbCollider* a, const AabbCollider* b);

    /// OBB vs OBB: 分離軸定理（SAT）
    static bool CheckObbVsObb(const ObbCollider* a, const ObbCollider* b);

    /// Circle vs Circle: 中心間距離²
    static bool CheckCircleVsCircle(const CircleCollider* a, const CircleCollider* b);

    /// AABB vs OBB: AABBの軸(2本) + OBBの軸(2本) = 計4本でSAT判定
    static bool CheckAabbVsObb(const AabbCollider* aabb, const ObbCollider* obb);

    /// <summary>
    /// AABB vs Circle: AABBの辺上で円の中心に最も近い点を求め、
    /// その点と円の中心の距離² ≤ r² で判定
    /// </summary>
    static bool CheckAabbVsCircle(const AabbCollider* aabb, const CircleCollider* circle);

    /// <summary>
    /// OBB vs Circle:
    ///   1. 円の中心をOBBのローカル座標系に変換（逆回転）
    ///   2. ローカル座標系でAABB vs Circle と同じ最近接点判定を行う
    /// </summary>
    static bool CheckObbVsCircle(const ObbCollider* obb, const CircleCollider* circle);


    // ===== SAT判定のユーティリティ =====

    /// <summary>
    /// 頂点集合を軸に射影し、最小値と最大値を返す。
    /// 射影 = 各頂点と軸の内積
    /// </summary>
    static void ProjectVertices(const std::vector<Vec2>& vertices, const Vec2& axis,
        float& outMin, float& outMax);

    /// <summary>
    /// 2つの区間 [minA, maxA] と [minB, maxB] が重なっているか
    /// </summary>
    static bool IntervalsOverlap(float minA, float maxA, float minB, float maxB);
};