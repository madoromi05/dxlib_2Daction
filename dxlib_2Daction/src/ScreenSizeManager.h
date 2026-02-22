#pragma once
#include "map/StageInformation.h"
#include "DxLib.h"

class ScreenSizeManager {
public:
    ScreenSizeManager();
    ~ScreenSizeManager();

    bool Initialize();
    void BeginDraw();
    void EndDraw();

    int GetLogicalWidth() const { return m_logicalWidth; }
    int GetLogicalHeight() const { return m_logicalHeight; }
private:
    int m_virtualScreenHandle; // 仮想スクリーンのハンドル
    int m_monitorWidth;        // 実際のモニターの幅
    int m_monitorHeight;       // 実際のモニターの高さ
    int m_logicalWidth;        // 横を保存する変数
    int m_logicalHeight;       // 縦を保存する変数
};  