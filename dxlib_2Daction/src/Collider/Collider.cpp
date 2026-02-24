#include "Collider.h"
#include "map/Map.h"
#include "map/StageInformation.h"
#include "DxLib.h"

Collider::Collider(float width, float height, float offsetX, float offsetY)
    : m_width(width)
    , m_height(height)
    , m_offsetX(offsetX)
    , m_offsetY(offsetY)
{}

bool Collider::IsCollidingWithMap(Map* map, float nextX, float nextY) const {
    if (map == nullptr) return false;

    // 当たり判定の矩形（四角形）の上下左右のピクセル座標を計算
    // -0.1f して、隣のマスにギリギリ触れている部分を「壁にめり込んだ」と誤認しないようにする
    float left = nextX + m_offsetX;
    float right = nextX + m_offsetX + m_width - 0.1f;
    float top = nextY + m_offsetY;
    float bottom = nextY + m_offsetY + m_height - 0.1f;

    // 矩形がまたがっているマス目（タイル）のインデックスを計算
    int startX = static_cast<int>(left) / stage_information::kMapGridSize;
    int endX = static_cast<int>(right) / stage_information::kMapGridSize;
    int startY = static_cast<int>(top) / stage_information::kMapGridSize;
    int endY = static_cast<int>(bottom) / stage_information::kMapGridSize;

    // 矩形がかぶっている全てのマス目に対して、壁かどうかチェックする
    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            // map->IsWall にはピクセル座標を渡す仕様なので、マスの左上座標に変換して渡す
            if (map->IsWall(x * stage_information::kMapGridSize, y * stage_information::kMapGridSize)) {
                return true; // 1つでも壁があれば衝突している
            }
        }
    }

    return false;
}

// デバッグ用の当たり判定矩形の描画
void Collider::DrawDebug(float currentX, float currentY, unsigned int color) const {
    int left = static_cast<int>(currentX + m_offsetX);
    int top = static_cast<int>(currentY + m_offsetY);
    int right = static_cast<int>(currentX + m_offsetX + m_width);
    int bottom = static_cast<int>(currentY + m_offsetY + m_height);

    // 四角形を描画（最後の引数 FALSE は「塗りつぶさずに枠線だけ描く」設定）
    DrawBox(left, top, right, bottom, color, FALSE);
}