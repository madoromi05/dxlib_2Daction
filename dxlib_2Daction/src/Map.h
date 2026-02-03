#pragma once
#include "DxLib.h"

// --- 定数定義 ---
constexpr int TILE_SIZE = 16;       // 1マスの大きさ（ピクセル）
constexpr int MAP_WIDTH = 20;       // 横のマス数 (320 / 16 = 20)
constexpr int MAP_HEIGHT = 20;      // 縦のマス数 (320 / 16 = 20)

// 画像分割用の設定
constexpr int TILE_X_NUM = 16;      // 画像の横のチップ数
constexpr int TILE_Y_NUM = 16;      // 画像の縦のチップ数
constexpr int TILE_TOTAL = TILE_X_NUM * TILE_Y_NUM; // 総チップ数

// --- タイルの種類 (Enum) ---
// 画像の並び順（インデックス）に合わせて値を設定します
enum class TileType {
    Floor = 0,  // 床（何もない）
    Wall = 1,  // 壁
    Block = 2,  // 壊せるブロックなど（仮）
    Trap = 3,  // 罠など（仮）
    None = -1  // 範囲外
};

class Map {
private:
    // 分割した画像のハンドルを保存する配列
    int _graphHandles[TILE_TOTAL];

    // マップ配置データ（int型のまま保持）
    int _mapData[MAP_HEIGHT][MAP_WIDTH];

public:
    Map();
    ~Map();

    // 初期化（画像の読み込み）
    void Initialize();

    // 描画
    void Draw();

    // 便利な判定関数
    TileType GetTileType(float x, float y); // 座標からタイルの種類を取得
    bool IsWall(float x, float y);          // その座標は壁か？
};