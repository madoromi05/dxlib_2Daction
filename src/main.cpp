#include "DxLib.h"
#include "SceneChange.h"
#include "Character.h"
int PlayerX, PlayerY;
int PlayerGraph;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 作業ディレクトリをresorceフォルダから参照するように設定
	SetCurrentDirectory("../resorce");
	// 画面モードのセット
	SetGraphMode(640, 480, 16);

	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		Run();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}