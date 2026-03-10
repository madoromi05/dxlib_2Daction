#include "MapCollider.h"
#include "map/Map.h"
#include "map/StageInformation.h"
#include <algorithm>

bool MapCollider::IsCollidingWithMap(AabbCollider& aabb, Map* map,
    float nextX, float nextY,
    bool isFacingLeft, float spriteWidth) {
    if (map == nullptr) return false;

    // AABBのワールド座標を更新
    aabb.SetWorldPosition(nextX, nextY, isFacingLeft, spriteWidth);

    // 当たり判定の矩形の端を取得
    // -0.1f して、隣のマスにギリギリ触れている部分を誤認しないようにする
    float left = aabb.GetLeft();
    float right = aabb.GetRight() - 0.1f;
    float top = aabb.GetTop();
    float bottom = aabb.GetBottom() - 0.1f;

    // 矩形がまたがっているタイルのインデックス範囲を計算
    int startX = static_cast<int>(left) / stage_information::kMapGridSize;
    int endX = static_cast<int>(right) / stage_information::kMapGridSize;
    int startY = static_cast<int>(top) / stage_information::kMapGridSize;
    int endY = static_cast<int>(bottom) / stage_information::kMapGridSize;

    // 矩形が重なっている全てのタイルに対して壁チェック
    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            float tileLeft = static_cast<float>(x * stage_information::kMapGridSize);
            float tileRight = tileLeft + stage_information::kMapGridSize - 0.1f;
            float tileTop = static_cast<float>(y * stage_information::kMapGridSize);
            float tileBottom = tileTop + stage_information::kMapGridSize - 0.1f;

            float overlapLeft = (std::max)(left, tileLeft);
            float overlapRight = (std::min)(right, tileRight);
            float overlapTop = (std::max)(top, tileTop);
            float overlapBottom = (std::min)(bottom, tileBottom);

            if (overlapLeft <= overlapRight && overlapTop <= overlapBottom) {
                if (map->IsWall(overlapLeft, overlapBottom) ||
                    map->IsWall(overlapRight, overlapBottom) ||
                    map->IsWall(overlapLeft, overlapTop) ||
                    map->IsWall(overlapRight, overlapTop)) {
                    return true;
                }
            }
        }
    }
    return false;
}