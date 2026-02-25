#include "MapLayers.h"
#include "Collider/Collider.h"

bool FileExists(const std::string& path) {
    std::ifstream fs(path);
    return fs.is_open();
}

// =========================================================
// BackgroundLayer の実装
// =========================================================
void BackgroundLayer::Initialize() {
    if (!FileExists(m_fileName)) {
        throw std::runtime_error("【致命的エラー】背景画像が見つかりません。\nパス: " + m_fileName);
    }
    m_graphHandle = LoadGraph(m_fileName.c_str());

    if (m_graphHandle == -1) {
        throw std::runtime_error("【致命的エラー】背景画像の読み込みに失敗しました。\nパス: " + m_fileName);
    }
}
void BackgroundLayer::Finalize() {
    if (m_graphHandle != -1) DeleteGraph(m_graphHandle);
}

void BackgroundLayer::RegisterTo(DrawableList* list) {
    list->Add(this, stage_information::kZOrderBackground);
}

void BackgroundLayer::Draw() const {
    if (m_graphHandle != -1) {
        int imgWidth, imgHeight;
        GetGraphSize(m_graphHandle, &imgWidth, &imgHeight);
        int targetHeight = stage_information::kStagePixelHeight;
        float scale = (float)targetHeight / imgHeight;
        int drawWidth = (int)(imgWidth * scale);
        int drawHeight = targetHeight;
        DrawExtendGraph(0, 0, drawWidth, drawHeight, m_graphHandle, FALSE);
    }
}

// ==========================================
// TileLayer の実装
// ==========================================
TileLayer::TileLayer()
    : m_isDebugMode(true)
{
    for (auto& h : m_tileHandles) h = -1;

    // マップデータ定義（枠だけ壁の例）
    for (int y = 0; y < stage_information::kMapHeight; y++) {
        for (int x = 0; x < stage_information::kMapWidth; x++) {
            m_mapData[y][x] = 0;
        }
    }

    // 全部のタイルを「通過可能 (false)」にする
    for (int i = 0; i < stage_information::kTileTotalNum; ++i) {
        m_collisionTable[i] = false;
    }

    // 壁や床にしたいタイルの画像番号をtrueにする
    m_collisionTable[3] = true;
}

void TileLayer::LoadMapData(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("【エラー】CSVファイルが見つかりません。\nパス: " + filePath);
    }
    std::string line;
    int y = 0;
    while (std::getline(file, line) && y < stage_information::kMapHeight) {
        std::stringstream ss(line);
        std::string value;
        int x = 0;
        while (std::getline(ss, value, ',') && x < stage_information::kMapWidth) {
            try { m_mapData[y][x] = std::stoi(value); }
            catch (...) { m_mapData[y][x] = 0; }
            x++;
        }
        y++;
    }
}

void TileLayer::Initialize() {
    int result = LoadDivGraph(
        ResourcePath::Map::kTileMap, 
        stage_information::kTileTotalNum,
        stage_information::kTileX,
        stage_information::kTileY,
        stage_information::kTileSize,
        stage_information::kTileSize,
        m_tileHandles);

    if (result == -1) {
        throw std::runtime_error("【エラー】タイルマップ画像が見つかりません。\nパス: " + std::string(ResourcePath::Map::kTileMap));
    }

    LoadMapData(ResourcePath::Map::kMapCSV1);
}

void TileLayer::Finalize() {
    for (auto h : m_tileHandles) {
        if (h != -1) DeleteGraph(h);
    }
}

void TileLayer::RegisterTo(DrawableList* list) {
    list->Add(this, stage_information::kZOrderTile); // タイルは背景より手前なので Zオーダー「10」
}

void TileLayer::Draw() const {
    for (int y = 0; y < stage_information::kMapHeight; y++) {
        for (int x = 0; x < stage_information::kMapWidth; x++) {
            int id = m_mapData[y][x];
            if (id >= 0 && m_tileHandles[id] != -1) {
                int drawX = x * stage_information::kMapGridSize;
                int drawY = y * stage_information::kMapGridSize;
                DrawExtendGraph(
                    drawX, drawY,
                    drawX + stage_information::kMapGridSize,
                    drawY + stage_information::kMapGridSize,
                    m_tileHandles[id], TRUE
                );

                if (m_isDebugMode && m_collisionTable[id]) {
                    // 当たり判定の枠線（赤色）を描画する
                    DrawBox(
                        drawX, drawY,
                        drawX + stage_information::kMapGridSize,
                        drawY + stage_information::kMapGridSize,
                        GetColor(255, 0, 0), FALSE
                    );
                }
            }
        }
    }
}

bool TileLayer::IsWall(float x, float y) {
    int xi = (int)x / stage_information::kMapGridSize;
    int yi = (int)y / stage_information::kMapGridSize;
    if (xi < 0 || xi >= stage_information::kMapWidth || yi < 0 || yi >= stage_information::kMapHeight) {
        return false;
    }

    int id = m_mapData[yi][xi];

    if (id < 0) {
        return false;
    }

    if (id >= stage_information::kTileTotalNum) {
        return true;
    }

    return m_collisionTable[id];
}

// ==========================================
// ObjectLayer の実装
// ==========================================
ObjectLayer::ObjectLayer() : m_treeImg(-1), m_bushImg(-1) {}

void ObjectLayer::Initialize() {
    m_treeImg = LoadGraph(ResourcePath::Map::kTree);
    m_bushImg = LoadGraph(ResourcePath::Map::kBush);

    if (m_treeImg != -1) m_objects.push_back(MapObjectData(50 * 2, 50 * 2, m_treeImg));
    if (m_bushImg != -1) m_objects.push_back(MapObjectData(100 * 2, 100 * 2, m_bushImg));
}

void ObjectLayer::Finalize() {
    if (m_treeImg != -1) DeleteGraph(m_treeImg);
    if (m_bushImg != -1) DeleteGraph(m_bushImg);
}

void ObjectLayer::RegisterTo(DrawableList* list) {
    for (auto& obj : m_objects) {
        list->Add(&obj, stage_information::kZOrderObjectBase + obj.y);
    }
}