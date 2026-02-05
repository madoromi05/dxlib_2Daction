#include "Map.h"
#include <stdexcept>
#include <string>

Map::Map() {
    // 画像ハンドルの初期化
    for (int i = 0; i < TILE_TOTAL; i++) {
        _graphHandles[i] = -1;
    }

    // --- マップデータの定義（仮） ---
    int tempMap[MAP_HEIGHT][MAP_WIDTH] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    };

    // メンバ変数へコピー
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            _mapData[y][x] = tempMap[y][x];
        }
    }
}

Map::~Map() {
    // 画像ハンドルの削除
    for (int i = 0; i < TILE_TOTAL; i++) {
        if (_graphHandles[i] != -1) {
            DeleteGraph(_graphHandles[i]);
        }
    }
}

void Map::Initialize() {
    // 画像分割読み込み
    // ファイル名は実際のプロジェクト内の画像ファイル名に合わせてください
    int result = LoadDivGraph("Tileset.png",
        TILE_TOTAL,
        TILE_X_NUM,
        TILE_Y_NUM,
        TILE_SIZE,
        TILE_SIZE,
        _graphHandles);

    if(result == -1) {
        std::string errorMsg = "画像の読み込みに失敗しました。\nファイル名: monochrome_tilemap.png\n";
        throw std::runtime_error(errorMsg);
    }
}

void Map::Draw() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int chipID = _mapData[y][x];

            // 描画座標の計算
            int drawX = x * TILE_SIZE;
            int drawY = y * TILE_SIZE;

            // 範囲内のIDなら描画
            if (chipID >= 0 && chipID < TILE_TOTAL) {
                DrawGraph(drawX, drawY, _graphHandles[chipID], TRUE);
            }
        }
    }
}

// 座標(x, y)にあるタイルの種類を返す
TileType Map::GetTileType(float x, float y) {
    // ピクセル座標をマップ配列のインデックスに変換
    int xIndex = (int)x / TILE_SIZE;
    int yIndex = (int)y / TILE_SIZE;

    // 画面外チェック
    if (xIndex < 0 || xIndex >= MAP_WIDTH || yIndex < 0 || yIndex >= MAP_HEIGHT) {
        return TileType::None;
    }

    // int型のIDをEnumに変換して返す
    return static_cast<TileType>(_mapData[yIndex][xIndex]);
}

// 壁（またはブロック）かどうかを判定する
bool Map::IsWall(float x, float y) {
    TileType type = GetTileType(x, y);

    // 壁またはブロックなら true を返す
    if (type == TileType::Wall || type == TileType::Block) {
        return true;
    }

    return false;
}