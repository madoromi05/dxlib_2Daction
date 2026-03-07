#include "SceneChanger.h"
#include "BattleScene.h"

SceneChanger::SceneChanger() : m_currentScene(nullptr) {
    ChangeScene(SceneType::Battle);
}

SceneChanger::~SceneChanger() {
    if (m_currentScene != nullptr) {
        m_currentScene->Finalize();
    }
}

void SceneChanger::ChangeScene(SceneType nextScene) {
    if (m_currentScene != nullptr) {
        m_currentScene->Finalize();
        m_currentScene.reset();
    }

    // 次のシーンを生成（ここで分岐）
    switch (nextScene) {
    case SceneType::Battle:
        m_currentScene = std::make_unique<BattleScene>();
        break;
    default:
        // エラー処理など
        break;
    }

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