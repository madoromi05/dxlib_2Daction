#pragma once

// ==========================================
// ステージ（マップ）の基本サイズや設定を管理
// ==========================================

namespace stage_information {

    // マップ全体の大きさを決める定数
    constexpr int kMapGridSize = 32;        // ゲーム画面上の1マスの大きさ
    constexpr int kMapWidth = 40;           // マップの横のマス数
    constexpr int kMapHeight = 23;          // マップの縦のマス数

    // 計算済みのステージ全体のピクセルサイズ
    constexpr int kStagePixelWidth  = kMapWidth * kMapGridSize;
    constexpr int kStagePixelHeight = kMapHeight * kMapGridSize;

	// ゲーム画面の大きさを決める定数
    constexpr int kCameraHeight = 600;

    // タイル画像に関する定数
    constexpr int kTileSize = 32;       // 元画像のタイル1枚のサイズ
    constexpr int kTileX = 12;          // タイルマップ画像の横の分割数
    constexpr int kTileY = 8;           // タイルマップ画像の縦の分割数
    constexpr int kTileTotalNum = kTileX * kTileY; // 画像の総分割数

	// Zオーダーの定数
    constexpr int kZOrderBackground = -100; // 最背面：遠景
    constexpr int kZOrderTile = -50;        // 地面・壁：タイルのレイヤー
    constexpr int kZOrderObjectBase = 0;    // 物体：これにY座標を足して、物体同士の前後を表現する
    constexpr int kZOrderUI = 100;          // 最前面：UIや文字など
}