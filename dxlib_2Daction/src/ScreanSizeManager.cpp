#include "ScreenSizeManager.h"
#include <algorithm>

ScreenSizeManager::ScreenSizeManager()
    : m_virtualScreenHandle(-1)
    , m_monitorWidth(0)
    , m_monitorHeight(0)
    , m_logicalHeight(stage_information::kCameraHeight)
{}

ScreenSizeManager::~ScreenSizeManager() {
    if (m_virtualScreenHandle != -1) {
        // DxLib_Endが呼ばれると自動で解放されるため、明示的にDeleteGraphしなくても良い
    }
}

bool ScreenSizeManager::Initialize() {
    // モニターサイズの取得
    m_monitorWidth = GetSystemMetrics(SM_CXSCREEN);
    m_monitorHeight = GetSystemMetrics(SM_CYSCREEN);

    // 基準の縦幅から、モニターの比率に合わせて横幅を計算する
    m_logicalHeight = stage_information::kCameraHeight;
    float aspect = static_cast<float>(m_monitorWidth) / static_cast<float>(m_monitorHeight);
    m_logicalWidth = static_cast<int>(m_logicalHeight * aspect);

    // DxLibの設定
    SetCurrentDirectory("resource");
    SetGraphMode(m_monitorWidth, m_monitorHeight, 32);
    ChangeWindowMode(TRUE);
    SetWindowStyleMode(7);

    // DxLib初期化
    if (DxLib_Init() == -1) {
        return false;
    }

    // 仮想スクリーンの作成
    m_virtualScreenHandle = MakeScreen(m_logicalWidth, m_logicalHeight, TRUE);
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

    float scaleX = (float)m_monitorWidth / m_logicalWidth;
    float scaleY = (float)m_monitorHeight / m_logicalHeight;
    float scale = (std::min)(scaleX, scaleY);

    int drawWidth = (int)(m_logicalWidth * scale);
    int drawHeight = (int)(m_logicalHeight * scale);

    int offsetX = (m_monitorWidth - drawWidth) / 2;
    int offsetY = (m_monitorHeight - drawHeight) / 2;

    DrawExtendGraph(offsetX, offsetY, offsetX + drawWidth, offsetY + drawHeight, m_virtualScreenHandle, FALSE);

    // 画面反映
    ScreenFlip();
}