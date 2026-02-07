#include "MapLayers.h"
#include <fstream> // ファイル存在チェック用
#include <string>
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
void BackgroundLayer::Finalize() {
    if (m_graphHandle != -1) DeleteGraph(m_graphHandle);
}
void BackgroundLayer::Draw(bool drawFront) {
    if (!drawFront && m_graphHandle != -1) {
        // 背景は画面いっぱいに描画
        DrawExtendGraph(0, 0, MAP_WIDTH * MAP_GRID_SIZE, MAP_HEIGHT * MAP_GRID_SIZE, m_graphHandle, FALSE);
    }
}

// ==========================================
// TileLayer の実装
// ==========================================
TileLayer::TileLayer() {
    // ハンドル初期化
    for (auto& h : m_tileHandles) h = -1;

    // マップデータ定義（枠だけ壁の例）
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (x == 0 || x == MAP_WIDTH - 1 || y == 0 || y == MAP_HEIGHT - 1)
                m_mapData[y][x] = 1; // 壁
            else
                m_mapData[y][x] = 0; // 床
        }
    }
}

void TileLayer::Initialize() {

    // 画像分割読み込み
    LoadDivGraph(
        ResourcePath::Map::kTileMap, 
        TILE_TOTAL_NUM,
        TILE_X,
        TILE_Y,
        TILE_SIZE,
        TILE_SIZE,
        m_tileHandles);
}

void TileLayer::Finalize() {
    for (auto h : m_tileHandles) {
        if (h != -1) DeleteGraph(h);
    }
}

void TileLayer::Draw(bool drawFront) {
    // タイルマップは基本的に「奥(false)」のタイミングで描画
    if (drawFront) return;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int id = m_mapData[y][x];
            if (id >= 0 && m_tileHandles[id] != -1) {
                int drawX = x * MAP_GRID_SIZE;
                int drawY = y * MAP_GRID_SIZE;

                DrawExtendGraph(
                    drawX,
                    drawY,
                    drawX + MAP_GRID_SIZE,
                    drawY + MAP_GRID_SIZE,
                    m_tileHandles[id],
                    TRUE
                );
            }
        }
    }
}

bool TileLayer::IsWall(float x, float y) {
    int xi = (int)x / 16;
    int yi = (int)y / 16;
    if (xi < 0 || xi >= MAP_WIDTH || yi < 0 || yi >= MAP_HEIGHT) return true;
    return (m_mapData[yi][xi] == 1); // 1は壁
}

// ==========================================
// ObjectLayer の実装
// ==========================================
ObjectLayer::ObjectLayer() : m_treeImg(-1), m_bushImg(-1) {}

void ObjectLayer::Initialize() {
    m_treeImg = LoadGraph(ResourcePath::Map::kTree);
    m_bushImg = LoadGraph(ResourcePath::Map::kBush);

    if (m_treeImg != -1) m_objects.push_back({ 50, 50, m_treeImg, false });
    if (m_bushImg != -1) m_objects.push_back({ 100, 100, m_bushImg, true });
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