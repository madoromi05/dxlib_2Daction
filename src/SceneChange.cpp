#include "SceneChanger.h"
#include "BattleScene.h"

SceneChanger::SceneChanger() : _currentScene(nullptr) {
    // 最初はバトルシーンから始めると仮定（あるいはタイトル）
    ChangeScene(SceneType::Battle);
}

SceneChanger::~SceneChanger() {
    if (_currentScene != nullptr) {
        _currentScene->Finalize();
        delete _currentScene;
        _currentScene = nullptr;
    }
}

void SceneChanger::ChangeScene(SceneType nextScene) {
    // 現在のシーンがあれば終了処理をして削除
    if (_currentScene != nullptr) {
        _currentScene->Finalize();
        delete _currentScene;
    }

    // 次のシーンを生成（ここで分岐）
    switch (nextScene) {
    case SceneType::Battle:
        _currentScene = new BattleScene();
        break;
    default:
        // エラー処理など
        break;
    }

    // 新しいシーンを初期化
    if (_currentScene != nullptr) {
        _currentScene->Initialize();
    }
}

void SceneChanger::Update() {
    if (_currentScene != nullptr) {
        _currentScene->Update(this);
    }
}

void SceneChanger::Draw() {
    if (_currentScene != nullptr) {
        _currentScene->Draw();
    }
}