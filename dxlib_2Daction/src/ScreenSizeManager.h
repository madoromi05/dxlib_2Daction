#pragma once
#include "DxLib.h"

class ScreenSizeManager {
public:
    static constexpr int kLogicalHeight = 240;

    ScreenSizeManager();
    ~ScreenSizeManager();

    bool Initialize();
    void BeginDraw();
    void EndDraw();

    int GetLogicalWidth() const { return m_logicalWidth; }
    int GetLogicalHeight() const { return kLogicalHeight; }
private:
    int m_virtualScreenHandle; // 仮想スクリーンのハンドル
    int m_monitorWidth;        // 実際のモニターの幅
    int m_monitorHeight;       // 実際のモニターの高さ
    int m_logicalWidth;        // ゲーム内の論理サイズ
};