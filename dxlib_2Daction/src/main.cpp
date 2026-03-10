#include "DxLib.h"
#include "Scene/SceneChanger.h"
#include "ScreenSizeManager.h"
#include <stdexcept>
#include <string>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    ScreenSizeManager screenSizeManager;

    if (!screenSizeManager.Initialize()) {
        return -1;
    }

    try {
        SceneChanger* sceneChanger = new SceneChanger();

        while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
        {
            screenSizeManager.BeginDraw();

            sceneChanger->Update();
            sceneChanger->Draw();
            screenSizeManager.EndDraw();
        }
    }
    catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "致命的なエラー", MB_OK | MB_ICONERROR);
    }

    // 後始末
    DxLib_End();

    return 0;
}
