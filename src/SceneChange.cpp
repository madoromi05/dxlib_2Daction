#include "SceneChange.h"
#include "Character.h"
#include "DxLib.h"

GameAdmin::GameAdmin() : m_pPlayer(nullptr) {
    // コンストラクタでは変数の初期化（nullptr代入など）を行う
}

GameAdmin::~GameAdmin() {
    // 終了時のメモリ解放
    if (m_pPlayer != nullptr) {
        delete m_pPlayer;
        m_pPlayer = nullptr;
    }
}

// グローバルなRun関数の実装
void Run() {
    static GameAdmin admin;
    static bool isInitialized = false;

    if (!isInitialized) {
        admin.Initialize();
        isInitialized = true;
    }

    admin.MainLoop();
}

void GameAdmin::Initialize() {
    SetDrawScreen(DX_SCREEN_BACK);
    m_pPlayer = new Character();
    m_pPlayer->Initialize();
}

void GameAdmin::MainLoop() {
    // 1. 更新
    m_pPlayer->Update();

    // 2. 描画
    ClearDrawScreen();  // 画面を初期化する
    m_pPlayer->Draw();
    ScreenFlip();
}