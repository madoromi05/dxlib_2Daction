#include "Map.h"

Map::Map() : m_tileLayer(nullptr) {}

Map::~Map() {
    Finalize();
}

void Map::Initialize(unsigned int seed) {
    // 遠景レイヤー生成
    m_layers.push_back(std::make_unique<BackgroundLayer>(ResourcePath::Map::kBackground));

    // タイルレイヤー生成
    auto tileLayer = std::make_unique<TileLayer>();
    m_tileLayer = tileLayer.get();
    m_layers.push_back(std::move(tileLayer));

	// オブジェクトレイヤー生成
	m_layers.push_back(std::make_unique<ObjectLayer>());

	// 全レイヤー初期化
	for (auto& layer : m_layers) {
		layer->Initialize();
	}

	int tempMapData[stage_information::kMapHeight][stage_information::kMapWidth];

	// マップ自動生成を実行
	m_generator.Generate(
		tempMapData,
		stage_information::kMapWidth,
		stage_information::kMapHeight,
		seed
	);

	// 生成されたデータを TileLayer にセット
	m_tileLayer->SetMapData(
		tempMapData,
		stage_information::kMapWidth,
		stage_information::kMapHeight
	);
}
bool Map::IsWall(float x, float y) {
    if (m_tileLayer) {
        return m_tileLayer->IsWall(x, y);
    }
    return false;
}

void Map::Finalize() {
    // 全レイヤー終了処理と削除
    for (auto& layer : m_layers) {
        layer->Finalize();
    }
    m_layers.clear();
    m_tileLayer = nullptr;
}

void Map::RegisterTo(DrawableList* list) {
    for (auto& layer : m_layers) {
        layer->RegisterTo(list);
    }
}
