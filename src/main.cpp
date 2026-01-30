#include "DxLib.h"
#include "SceneChanger.h"
#include "Character.h"

constexpr int kScreenWidth = 1920;
constexpr int kScreenHeight = 1080;
constexpr int kColorBit = 16;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	SetCurrentDirectory("../resorce");
	// 画面モードのセット
	SetGraphMode(kScreenWidth, kScreenHeight, kColorBit);

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// シーン管理者の生成
	SceneChanger* sceneChanger = new SceneChanger();

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();
		// 更新処理
		sceneChanger->Update();

		// 描画処理
		sceneChanger->Draw();

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}