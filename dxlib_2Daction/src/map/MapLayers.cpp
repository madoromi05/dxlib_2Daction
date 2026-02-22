#include "MapLayers.h"

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
        throw std::runtime_error("【致命的エラー】背景画像の読み込みに失敗しました(LoadGraphエラー)。\nパス: " + m_fileName);
    }
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
            try {
                m_mapData[y][x] = std::stoi(value);
            }
            catch (...) {
                m_mapData[y][x] = 0;
            }
            x++;
        }
        y++;
    }
}

void BackgroundLayer::Finalize() {
    if (m_graphHandle != -1) DeleteGraph(m_graphHandle);
}

void BackgroundLayer::Draw(bool drawFront) {
    if (!drawFront && m_graphHandle != -1) {
        // 画像の本来のサイズを取得
        int imgWidth, imgHeight;
        GetGraphSize(m_graphHandle, &imgWidth, &imgHeight);

        // 目標とする「高さ」（マップの高さ）
        int targetHeight = stage_information::kStagePixelHeight;

        // 高さを基準に倍率を計算（ マップの高さ ÷ 画像の高さ ）
        float scale = (float)targetHeight / imgHeight;

        // 新しい幅を計算（比率維持）
        // 高さはマップにピッタリ合わせ、幅はその倍率で自動調整
        int drawWidth = (int)(imgWidth * scale);
        int drawHeight = targetHeight;

        DrawExtendGraph(0, 0, drawWidth, drawHeight, m_graphHandle, FALSE);
    }
}
// ==========================================
// TileLayer の実装
// ==========================================
TileLayer::TileLayer() {
    // ハンドル初期化
    for (auto& h : m_tileHandles) h = -1;

    // マップデータ定義（枠だけ壁の例）
    for (int y = 0; y < stage_information::kMapHeight; y++) {
        for (int x = 0; x < stage_information::kMapWidth; x++) {
            m_mapData[y][x] = 0;
        }
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

void TileLayer::Draw(bool drawFront) {
    // タイルマップは基本的に「奥(false)」のタイミングで描画
    if (drawFront) return;

    for (int y = 0; y < stage_information::kMapHeight; y++) {
        for (int x = 0; x < stage_information::kMapWidth; x++) {
            int id = m_mapData[y][x];
            if (id >= 0 && m_tileHandles[id] != -1) {
                int drawX = x * stage_information::kMapGridSize;
                int drawY = y * stage_information::kMapGridSize;

                DrawExtendGraph(
                    drawX,
                    drawY,
                    drawX + stage_information::kMapGridSize,
                    drawY + stage_information::kMapGridSize,
                    m_tileHandles[id],
                    TRUE
                );
            }
        }
    }
}

bool TileLayer::IsWall(float x, float y) {
    int xi = (int)x / stage_information::kMapGridSize;
    int yi = (int)y / stage_information::kMapGridSize;
    if (xi < 0 || xi >= stage_information::kMapWidth || yi < 0 || yi >= stage_information::kMapHeight) return true;
    return (m_mapData[yi][xi] == 3);
}

// ==========================================
// ObjectLayer の実装
// ==========================================
ObjectLayer::ObjectLayer() : m_treeImg(-1), m_bushImg(-1) {}

void ObjectLayer::Initialize() {
    m_treeImg = LoadGraph(ResourcePath::Map::kTree);
    m_bushImg = LoadGraph(ResourcePath::Map::kBush);

    if (m_treeImg != -1) m_objects.push_back({ 50 * 2, 50 * 2, m_treeImg, false });
    if (m_bushImg != -1) m_objects.push_back({ 100 * 2, 100 * 2, m_bushImg, true });
}

void ObjectLayer::Finalize() {
    if (m_treeImg != -1) DeleteGraph(m_treeImg);
    if (m_bushImg != -1) DeleteGraph(m_bushImg);
}

void ObjectLayer::Draw(bool drawFront) {
    // 引数に応じて「手前」か「奥」のオブジェクトだけを選んで描画
    for (const auto& obj : m_objects) {
        if (obj.isFront == drawFront) {
            DrawGraph(obj.x, obj.y, obj.graphHandle, TRUE);
        }
    }
}