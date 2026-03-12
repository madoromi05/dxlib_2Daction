#pragma once
#include "DxLib.h"
#include "ResourcePath.h"
#include "StageInformation.h"
#include "DrawInterface/DrawableList.h"
#include "DrawInterface/IDrawable.h"
#include <vector>
#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <tchar.h>

#ifndef TSTRING_DEFINED
#define TSTRING_DEFINED
typedef std::basic_string<TCHAR> tstring;
#endif

struct MapObjectData : public IDrawable {
    int m_x;
    int m_y;
    int m_graphHandle;
    MapObjectData(int posX, int posY, int handle)
        : m_x(posX), m_y(posY), m_graphHandle(handle) {
    }

    void Draw(float cameraX, float cameraY) const override {
        int drawX = m_x - static_cast<int>(cameraX);
        int drawY = m_y - static_cast<int>(cameraY);
        DrawGraph(drawX, drawY, m_graphHandle, TRUE);
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
	tstring m_fileName;
public:
    BackgroundLayer(const tstring& fileName) : m_graphHandle(-1), m_fileName(fileName) {}
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
    // 壁判定
    bool IsWall(float x, float y);
	void SetMapData(int mapData[stage_information::kMapHeight][stage_information::kMapWidth], int width, int height);

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

