#include "Map.h"

Map::Map() : m_tileLayer(nullptr) {}

Map::~Map() {
    Finalize();
}

void Map::Initialize() {
    // 遠景レイヤー生成
    m_layers.push_back(new BackgroundLayer(ResourcePath::Map::kBackground));

    // タイルレイヤー生成
    m_tileLayer = new TileLayer();
    m_layers.push_back(m_tileLayer);

    // オブジェクトレイヤー生成
    m_layers.push_back(new ObjectLayer());

    // 全レイヤー初期化
    for (auto layer : m_layers) {
        layer->Initialize();
    }
}

bool Map::IsWall(float x, float y) {
    if (m_tileLayer) {
        return m_tileLayer->IsWall(x, y);
    }
    return false;
}

void Map::Finalize() {
    // 全レイヤー終了処理と削除
    for (auto layer : m_layers) {
        layer->Finalize();
        delete layer;
    }
    m_layers.clear();
    m_tileLayer = nullptr;
}

void Map::RegisterTo(DrawableList* list) {
    for (auto layer : m_layers) {
        layer->RegisterTo(list);
    }
}