#pragma once
#include "MapLayers.h"
#include "ResourcePath.h"
#include <vector>
#include <memory>

class Map {
private:
    // レイヤーのリスト（親クラスのポインタで管理！）
    // 背景、オブジェクト等は汎用的に管理
    std::vector<std::unique_ptr<MapLayer>> m_layers;

    // タイルレイヤーだけは当たり判定で頻繁に使うので、
    // 専用のポインタとしても持っておくと便利
    TileLayer* m_tileLayer;

public:
    Map();
    ~Map();

    void Initialize();
    void Finalize();

    // 壁判定（TileLayerへ委譲）
    bool IsWall(float x, float y);
    void RegisterTo(DrawableList* list);
};