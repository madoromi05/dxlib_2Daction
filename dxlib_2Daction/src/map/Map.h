#pragma once
#include "MapLayers.h"
#include "ResorcePath.h"
#include <vector>
#include <memory>

class Map {
private:
    // レイヤーのリスト（親クラスのポインタで管理！）
    // 背景、オブジェクト等は汎用的に管理
    std::vector<MapLayer*> m_layers;

    // タイルレイヤーだけは当たり判定で頻繁に使うので、
    // 専用のポインタとしても持っておくと便利
    TileLayer* m_tileLayer;

public:
    Map();
    ~Map();

    void Initialize();
    void Finalize();

    // 描画：奥を描くモードと、手前を描くモード
    void DrawBack();
    void DrawFront();

    // 壁判定（TileLayerへ委譲）
    bool IsWall(float x, float y);
};