#include "ScreenSizeManager.h"
#include <algorithm>

ScreenSizeManager::ScreenSizeManager()
    : m_virtualScreenHandle(-1), m_monitorWidth(0), m_monitorHeight(0), m_logicalWidth(320) {
}

ScreenSizeManager::~ScreenSizeManager() {
    if (m_virtualScreenHandle != -1) {
        // DxLib_Endが呼ばれると自動で解放されるため、明示的にDeleteGraphしなくても良い
    }
}

bool ScreenSizeManager::Initialize() {
    // 1. モニターサイズの取得
    m_monitorWidth = GetSystemMetrics(SM_CXSCREEN);
    m_monitorHeight = GetSystemMetrics(SM_CYSCREEN);
    m_logicalWidth = kLogicalHeight * m_monitorWidth / m_monitorHeight;

    // 2. DxLibの設定
    SetCurrentDirectory("resorce");
    SetGraphMode(m_monitorWidth, m_monitorHeight, 32); // 32bitカラー推奨
    ChangeWindowMode(TRUE);
    SetWindowStyleMode(7); // 枠なしウィンドウ

    // 3. DxLib初期化
    if (DxLib_Init() == -1) {
        return false;
    }

    // 4. 仮想スクリーンの作成
    m_virtualScreenHandle = MakeScreen(m_logicalWidth, kLogicalHeight, TRUE);

    // ドット絵をくっきりさせる設定
    SetDrawMode(DX_DRAWMODE_NEAREST);

    return true;
}

void ScreenSizeManager::BeginDraw() {
    // 描画先を仮想スクリーン（320x240）にする
    SetDrawScreen(m_virtualScreenHandle);
    ClearDrawScreen();
}

void ScreenSizeManager::EndDraw() {
    // 描画先を裏画面（モニター全体）に戻す
    SetDrawScreen(DX_SCREEN_BACK);
    ClearDrawScreen(); // 黒帯部分をクリアするために必要

    //// 拡大率の計算
    //float scaleX = (float)m_monitorWidth / kGameLogicalWidth;
    //float scaleY = (float)m_monitorHeight / kGameLogicalHeight;
    //// 小さい方の倍率に合わせて、画面からはみ出さないようにする
    //float scale = (scaleX < scaleY) ? scaleX : scaleY;

    //// 描画サイズと位置（画面中央）を計算
    //int finalW = (int)(kGameLogicalWidth * scale);
    //int finalH = (int)(kGameLogicalHeight * scale);
    //int posX = (m_monitorWidth - finalW) / 2;
    //int posY = (m_monitorHeight - finalH) / 2;

    //// 仮想スクリーンを引き伸ばして描画
    //DrawExtendGraph(posX, posY, posX + finalW, posY + finalH, m_virtualScreenHandle, FALSE);
    DrawExtendGraph(0, 0, m_monitorWidth, m_monitorHeight, m_virtualScreenHandle, FALSE);
    // 画面反映
    ScreenFlip();
}