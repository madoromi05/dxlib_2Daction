#pragma once
#include "MapLayers.h"
#include "ResourcePath.h"
#include <vector>
#include <memory>
#include "MapGenerator.h"
class Map {
private:
    // レイヤーのリスト（親クラスのポインタで管理！）
    // 背景、オブジェクト等は汎用的に管理
    std::vector<std::unique_ptr<MapLayer>> m_layers;

    // 当たり判定用
    TileLayer* m_tileLayer;
	MapGenerator m_generator;
public:
    Map();
    ~Map();

    void Initialize(unsigned int seed);
    void Finalize();

    // 壁判定（TileLayerへ委譲）
    bool IsWall(float x, float y);
    void RegisterTo(DrawableList* list);

	int GetStartX() const { return m_generator.GetStartX(); }
	int GetStartY() const { return m_generator.GetStartY(); }
	const std::vector<RoomRect>& GetRooms() const { return m_generator.GetRooms(); }
};
