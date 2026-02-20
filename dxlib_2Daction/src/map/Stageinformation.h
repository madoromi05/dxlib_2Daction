#pragma once

// ==========================================
// ステージ（マップ）の基本サイズや設定を管理
// ==========================================

namespace stage_information {

    // マップ全体の大きさを決める定数
    constexpr int kMapGridSize = 32;        // ゲーム画面上の1マスの大きさ
    constexpr int kMapWidth = 26;           // マップの横のマス数
    constexpr int kMapHeight = 15;          // マップの縦のマス数

    // 計算済みのステージ全体のピクセルサイズ
    constexpr int kStagePixelWidth = kMapWidth * kMapGridSize;
    constexpr int kStagePixelHeight = kMapHeight * kMapGridSize;

    // タイル画像に関する定数
    constexpr int kTileSize = 32;       // 元画像のタイル1枚のサイズ
    constexpr int kTileX = 12;          // タイルマップ画像の横の分割数
    constexpr int kTileY = 8;           // タイルマップ画像の縦の分割数
    constexpr int kTileTotalNum = kTileX * kTileY; // 画像の総分割数

}