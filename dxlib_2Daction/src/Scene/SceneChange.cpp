#include "SceneChanger.h"
#include "BattleScene.h"

SceneChanger::SceneChanger() : m_currentScene(nullptr) {
    // 最初はバトルシーンから始めると仮定（あるいはタイトル）
    ChangeScene(SceneType::Battle);
}

SceneChanger::~SceneChanger() {
    if (m_currentScene != nullptr) {
        m_currentScene->Finalize();
        delete m_currentScene;
        m_currentScene = nullptr;
    }
}

void SceneChanger::ChangeScene(SceneType nextScene) {
    // 現在のシーンがあれば終了処理をして削除
    if (m_currentScene != nullptr) {
        m_currentScene->Finalize();
        delete m_currentScene;
    }

    // 次のシーンを生成（ここで分岐）
    switch (nextScene) {
    case SceneType::Battle:
        m_currentScene = new BattleScene();
        break;
    default:
        // エラー処理など
        break;
    }

    // 新しいシーンを初期化
    if (m_currentScene != nullptr) {
        m_currentScene->Initialize();
    }
}

void SceneChanger::Update() {
    if (m_currentScene != nullptr) {
        m_currentScene->Update(this);
    }
}

void SceneChanger::Draw() {
    if (m_currentScene != nullptr) {
        m_currentScene->Draw();
    }
}