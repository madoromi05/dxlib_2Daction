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
    // モニターサイズの取得
    m_monitorWidth = GetSystemMetrics(SM_CXSCREEN);
    m_monitorHeight = GetSystemMetrics(SM_CYSCREEN);
    m_logicalWidth = kLogicalHeight * m_monitorWidth / m_monitorHeight;

    // DxLibの設定
    SetCurrentDirectory("resorce");
    SetGraphMode(m_monitorWidth, m_monitorHeight, 32);
    ChangeWindowMode(TRUE);
    SetWindowStyleMode(7);

    // DxLib初期化
    if (DxLib_Init() == -1) {
        return false;
    }

    // 仮想スクリーンの作成
    m_virtualScreenHandle = MakeScreen(m_logicalWidth, kLogicalHeight, TRUE);

    // ドット絵をくっきりさせる設定
    SetDrawMode(DX_DRAWMODE_NEAREST);

    return true;
}

void ScreenSizeManager::BeginDraw() {
    SetDrawScreen(m_virtualScreenHandle);
    ClearDrawScreen();
}

void ScreenSizeManager::EndDraw() {
    SetDrawScreen(DX_SCREEN_BACK);
    ClearDrawScreen();
    DrawExtendGraph(0, 0, m_monitorWidth, m_monitorHeight, m_virtualScreenHandle, FALSE);
    // 画面反映
    ScreenFlip();
}