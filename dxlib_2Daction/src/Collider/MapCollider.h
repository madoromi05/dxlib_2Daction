#pragma once
#include "AabbCollider.h"

class Map;

/// <summary>
/// 地形（マップタイル）との衝突判定ユーティリティ
/// </summary>
class MapCollider {
public:
    MapCollider() = delete;

    /// <summary>
    /// 指定座標に AabbCollider を配置した場合に、マップの壁にぶつかるか判定する。
    /// </summary>
    /// <param name="isFacingLeft">左向きかどうか</param>
    static bool IsCollidingWithMap(AabbCollider& aabb, Map* map,
        float nextX, float nextY,
        bool isFacingLeft, float spriteWidth);
};