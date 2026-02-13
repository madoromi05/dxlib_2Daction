#pragma once
#include "DxLib.h"
#include "ResorcePath.h"
#include <vector>
#include <string>
#include <stdexcept>

constexpr int MAP_GRID_SIZE = 16;       // ゲーム画面上の1マスの大きさ
constexpr int MAP_WIDTH  = 20;          // マップの横マス数
constexpr int MAP_HEIGHT = 15;          // マップの縦マス数

// --- 定数や構造体の定義 ---
constexpr int TILE_SIZE = 32;           // タイル1枚のサイズ
constexpr int TILE_X = 12;              // タイルマップ横幅
constexpr int TILE_Y = 8;               // タイルマップ縦幅
constexpr int TILE_TOTAL_NUM = TILE_X * TILE_Y; // 画像分割総数

struct MapObjectData {
    int x, y;
    int graphHandle;
    bool isFront; // true:キャラの手前, false:キャラの奥
};

// --------------------------------------------------
// 基底クラス: MapLayer
// --------------------------------------------------
class MapLayer {
public:
    virtual ~MapLayer() = default;
    virtual void Initialize() = 0;
    virtual void Finalize() = 0;
    virtual void Draw(bool drawFront = false) = 0;
};

// --------------------------------------------------
// 遠景レイヤー (BackgroundLayer)
// --------------------------------------------------
class BackgroundLayer : public MapLayer {
private:
    int m_graphHandle;
    std::string m_fileName;
public:
    BackgroundLayer(const std::string& fileName) : m_graphHandle(-1), m_fileName(fileName) {}
    void Initialize() override;
    void Finalize() override;
    void Draw(bool drawFront) override;
};

// --------------------------------------------------
// タイルマップレイヤー (TileLayer)
// --------------------------------------------------
class TileLayer : public MapLayer {
private:
    int m_tileHandles[TILE_TOTAL_NUM];     // 画像ハンドル配列
    int m_mapData[MAP_HEIGHT][MAP_WIDTH]; // マップ配置データ

public:
    TileLayer();
    void Initialize() override;
    void Finalize() override;
    void Draw(bool drawFront) override;
    void LoadMapData(const std::string& filePath);
    // 壁判定
    bool IsWall(float x, float y);
};

// --------------------------------------------------
// オブジェクトレイヤー (ObjectLayer)
// --------------------------------------------------
class ObjectLayer : public MapLayer {
private:
    std::vector<MapObjectData> m_objects;
    int m_treeImg;
    int m_bushImg;
public:
    ObjectLayer();
    void Initialize() override;
    void Finalize() override;
    void Draw(bool drawFront) override;
};

