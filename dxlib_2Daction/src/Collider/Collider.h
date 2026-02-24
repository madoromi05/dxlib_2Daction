#pragma once

class Map;

class Collider {
public:
    // コンストラクタ（幅、高さ、画像左上からのXズレ、Yズレ）
    Collider(float width, float height, float offsetX = 0.0f, float offsetY = 0.0f);
    ~Collider() = default;

    // 次の座標に移動したとき、マップの壁（床・天井含む）にぶつかるか判定
    bool IsCollidingWithMap(class Map* map, float nextX, float nextY) const;
    void DrawDebug(float currentX, float currentY, unsigned int color) const;
private:
    float m_width;
    float m_height;
    float m_offsetX;
    float m_offsetY;
};