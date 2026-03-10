#include "CollisionHelper.h"
#include <algorithm>
#include <cmath>

// ============================================================
// メインディスパッチ
// ============================================================
bool CollisionHelper::CheckCollision(const ICollider* a, const ICollider* b) {
    if (a == nullptr || b == nullptr) return false;

    ShapeType typeA = a->GetShapeType();
    ShapeType typeB = b->GetShapeType();

    // 同種判定
    if (typeA == ShapeType::Aabb && typeB == ShapeType::Aabb) {
        return CheckAabbVsAabb(
            static_cast<const AabbCollider*>(a),
            static_cast<const AabbCollider*>(b));
    }
    if (typeA == ShapeType::Obb && typeB == ShapeType::Obb) {
        return CheckObbVsObb(
            static_cast<const ObbCollider*>(a),
            static_cast<const ObbCollider*>(b));
    }
    if (typeA == ShapeType::Circle && typeB == ShapeType::Circle) {
        return CheckCircleVsCircle(
            static_cast<const CircleCollider*>(a),
            static_cast<const CircleCollider*>(b));
    }

    // 異種判定（順序を正規化して呼び出す）
    if (typeA == ShapeType::Aabb && typeB == ShapeType::Obb) {
        return CheckAabbVsObb(
            static_cast<const AabbCollider*>(a),
            static_cast<const ObbCollider*>(b));
    }
    if (typeA == ShapeType::Obb && typeB == ShapeType::Aabb) {
        return CheckAabbVsObb(
            static_cast<const AabbCollider*>(b),
            static_cast<const ObbCollider*>(a));
    }

    if (typeA == ShapeType::Aabb && typeB == ShapeType::Circle) {
        return CheckAabbVsCircle(
            static_cast<const AabbCollider*>(a),
            static_cast<const CircleCollider*>(b));
    }
    if (typeA == ShapeType::Circle && typeB == ShapeType::Aabb) {
        return CheckAabbVsCircle(
            static_cast<const AabbCollider*>(b),
            static_cast<const CircleCollider*>(a));
    }

    if (typeA == ShapeType::Obb && typeB == ShapeType::Circle) {
        return CheckObbVsCircle(
            static_cast<const ObbCollider*>(a),
            static_cast<const CircleCollider*>(b));
    }
    if (typeA == ShapeType::Circle && typeB == ShapeType::Obb) {
        return CheckObbVsCircle(
            static_cast<const ObbCollider*>(b),
            static_cast<const CircleCollider*>(a));
    }

    return false;
}

// ============================================================
// 1. AABB vs AABB
// 各軸の区間が両方とも重なっていれば衝突
// ============================================================
bool CollisionHelper::CheckAabbVsAabb(const AabbCollider* a, const AabbCollider* b) {
    if (a->GetRight() <= b->GetLeft())   return false;
    if (a->GetLeft() >= b->GetRight())  return false;
    if (a->GetBottom() <= b->GetTop())    return false;
    if (a->GetTop() >= b->GetBottom()) return false;
    return true;
}

// ============================================================
// 2. OBB vs OBB — 分離軸定理（SAT）
//
// アルゴリズム:
//   (1) 両OBBから分離軸候補を取得（各2本、計4本）
//   (2) 各軸について、両OBBの全頂点を軸に射影（内積）
//   (3) 射影区間が1本でも重ならなければ分離軸が存在 → 非衝突
//   (4) 全ての軸で重なっていれば衝突
// ============================================================
bool CollisionHelper::CheckObbVsObb(const ObbCollider* a, const ObbCollider* b) {
    std::vector<Vec2> vertsA = a->GetWorldVertices();
    std::vector<Vec2> vertsB = b->GetWorldVertices();
    std::vector<Vec2> axesA = a->GetSeparatingAxes();
    std::vector<Vec2> axesB = b->GetSeparatingAxes();

    // OBB-A の分離軸で判定
    for (const auto& axis : axesA) {
        float minA, maxA, minB, maxB;
        ProjectVertices(vertsA, axis, minA, maxA);
        ProjectVertices(vertsB, axis, minB, maxB);
        if (!IntervalsOverlap(minA, maxA, minB, maxB)) return false;
    }

    // OBB-B の分離軸で判定
    for (const auto& axis : axesB) {
        float minA, maxA, minB, maxB;
        ProjectVertices(vertsA, axis, minA, maxA);
        ProjectVertices(vertsB, axis, minB, maxB);
        if (!IntervalsOverlap(minA, maxA, minB, maxB)) return false;
    }

    return true;
}

// ============================================================
// 3. Circle vs Circle
// 中心間距離² ≤ (r1 + r2)² で判定。平方根を回避して高速化。
// ============================================================
bool CollisionHelper::CheckCircleVsCircle(const CircleCollider* a, const CircleCollider* b) {
    Vec2 diff = a->GetWorldCenter() - b->GetWorldCenter();
    float distSq = diff.LengthSquared();
    float radiusSum = a->GetRadius() + b->GetRadius();
    return distSq <= radiusSum * radiusSum;
}

// ============================================================
// 4. AABB vs OBB — SAT
// AABBの分離軸(2本) + OBBの分離軸(2本) = 計4本で判定
// ============================================================
bool CollisionHelper::CheckAabbVsObb(const AabbCollider* aabb, const ObbCollider* obb) {
    std::vector<Vec2> vertsA = aabb->GetWorldVertices();
    std::vector<Vec2> vertsB = obb->GetWorldVertices();
    std::vector<Vec2> axesA = aabb->GetSeparatingAxes();
    std::vector<Vec2> axesB = obb->GetSeparatingAxes();

    for (const auto& axis : axesA) {
        float minA, maxA, minB, maxB;
        ProjectVertices(vertsA, axis, minA, maxA);
        ProjectVertices(vertsB, axis, minB, maxB);
        if (!IntervalsOverlap(minA, maxA, minB, maxB)) return false;
    }

    for (const auto& axis : axesB) {
        float minA, maxA, minB, maxB;
        ProjectVertices(vertsA, axis, minA, maxA);
        ProjectVertices(vertsB, axis, minB, maxB);
        if (!IntervalsOverlap(minA, maxA, minB, maxB)) return false;
    }

    return true;
}

// ============================================================
// 5. AABB vs Circle
//
// アルゴリズム:
//   (1) 円の中心座標を、AABBの範囲にクランプして最近接点を求める
//       clampedX = clamp(circle.center.x, aabb.left, aabb.right)
//       clampedY = clamp(circle.center.y, aabb.top, aabb.bottom)
//   (2) 最近接点と円の中心の距離² ≤ r² なら衝突
// ============================================================
bool CollisionHelper::CheckAabbVsCircle(const AabbCollider* aabb, const CircleCollider* circle) {
    Vec2 center = circle->GetWorldCenter();
    float radius = circle->GetRadius();

    // AABBの範囲に円の中心をクランプして最近接点を求める
    float closestX = (std::max)(aabb->GetLeft(), (std::min)(center.m_x, aabb->GetRight()));
    float closestY = (std::max)(aabb->GetTop(), (std::min)(center.m_y, aabb->GetBottom()));

    Vec2 diff = Vec2(closestX, closestY) - center;
    return diff.LengthSquared() <= radius * radius;
}

// ============================================================
// 6. OBB vs Circle
//
// アルゴリズム:
//   (1) 円の中心を OBBのローカル座標系に変換する
//       - OBBの中心を原点に移動
//       - OBBの回転角度の逆回転を適用
//       → ローカル座標系では OBB は軸平行矩形として扱える
//   (2) ローカル座標系で AABB vs Circle と同じ最近接点判定を行う
//       - 矩形の範囲 [-halfW, halfW] × [-halfH, halfH] にクランプ
//   (3) 最近接点と円の中心の距離² ≤ r² なら衝突
// ============================================================
bool CollisionHelper::CheckObbVsCircle(const ObbCollider* obb, const CircleCollider* circle) {
    Vec2 obbCenter = obb->GetWorldCenter();
    Vec2 circleCenter = circle->GetWorldCenter();
    float radius = circle->GetRadius();

    // (1) 円の中心をOBBのローカル座標系に変換
    Vec2 diff = circleCenter - obbCenter;

    // OBBの分離軸（= OBBのローカル軸）を取得
    std::vector<Vec2> axes = obb->GetSeparatingAxes();
    Vec2 axisX = axes[0];  // OBBのX軸方向
    Vec2 axisY = axes[1];  // OBBのY軸方向

    // diffをOBBのローカル軸に射影してローカル座標を求める
    float localX = Vec2::Dot(diff, axisX);
    float localY = Vec2::Dot(diff, axisY);

    // (2) OBBの頂点からhalf extentsを逆算
    // OBBの頂点[0]と頂点[1]の距離 / 2 = halfWidth
    // OBBの頂点[0]と頂点[3]の距離 / 2 = halfHeight
    std::vector<Vec2> verts = obb->GetWorldVertices();
    float halfW = (verts[1] - verts[0]).Length() * 0.5f;
    float halfH = (verts[3] - verts[0]).Length() * 0.5f;

    // ローカル座標系での最近接点をクランプで求める
    float closestX = (std::max)(-halfW, (std::min)(localX, halfW));
    float closestY = (std::max)(-halfH, (std::min)(localY, halfH));

    // (3) 最近接点と円の中心の距離判定
    float dx = localX - closestX;
    float dy = localY - closestY;
    return (dx * dx + dy * dy) <= radius * radius;
}

// ============================================================
// SAT ユーティリティ
// ============================================================
void CollisionHelper::ProjectVertices(const std::vector<Vec2>& vertices, const Vec2& axis,
    float& outMin, float& outMax) {
    // 最初の頂点で初期化
    outMin = Vec2::Dot(vertices[0], axis);
    outMax = outMin;

    // 残りの頂点を射影して最小・最大を更新
    for (size_t i = 1; i < vertices.size(); ++i) {
        float proj = Vec2::Dot(vertices[i], axis);
        if (proj < outMin) outMin = proj;
        if (proj > outMax) outMax = proj;
    }
}

bool CollisionHelper::IntervalsOverlap(float minA, float maxA, float minB, float maxB) {
    // 区間 [minA, maxA] と [minB, maxB] が重なっているか
    return maxA >= minB && maxB >= minA;
}