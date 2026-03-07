#pragma once
#include "DxLib.h"
#include "ResorcePath.h"
#include "StageInformation.h"
#include "DrowInterface/DrawableList.h"
#include "DrowInterface/IDrawable.h"
#include <vector>
#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

struct MapObjectData : public IDrawable {
    int x, y;
    int graphHandle;
    MapObjectData(int _x, int _y, int _handle) : x(_x), y(_y), graphHandle(_handle) {}

    void Draw(float cameraX, float cameraY) const override {
		int drawX = x - static_cast<int>(cameraX);
		int drawY = y - static_cast<int>(cameraY);
        DrawGraph(drawX, drawY, graphHandle, TRUE);
    }
};

// --------------------------------------------------
// 基底クラス: MapLayer
// --------------------------------------------------
class MapLayer {
public:
    virtual ~MapLayer() = default;
    virtual void Initialize() = 0;
    virtual void Finalize() = 0;
    virtual void RegisterTo(DrawableList* list) = 0;
};

// --------------------------------------------------
// 遠景レイヤー (BackgroundLayer)
// --------------------------------------------------
class BackgroundLayer : public MapLayer, public IDrawable {
private:
    int m_graphHandle;
    std::string m_fileName;
public:
    BackgroundLayer(const std::string& fileName) : m_graphHandle(-1), m_fileName(fileName) {}
    void Initialize() override;
    void Finalize() override;
    void RegisterTo(DrawableList* list) override;
    void Draw(float cameraX, float cameraY) const override;
};

// --------------------------------------------------
// タイルマップレイヤー (TileLayer)
// --------------------------------------------------
class TileLayer : public MapLayer, public IDrawable {
private:
    int m_tileHandles[stage_information::kTileTotalNum]{};                        // 画像ハンドル配列
	int m_mapData[stage_information::kMapHeight][stage_information::kMapWidth];   // マップ配置データ
    int m_collisionTable[stage_information::kTileTotalNum]{};
    bool m_isDebugMode;

public:
    TileLayer();
    void Initialize() override;
    void Finalize() override;
    void RegisterTo(DrawableList* list) override;
    void Draw(float cameraX, float cameraY) const override;
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
    void RegisterTo(DrawableList* list) override;
};

