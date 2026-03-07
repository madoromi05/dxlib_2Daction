#pragma once
#include <memory>
#include "IScene.h"

class SceneChanger {
private:
    std::unique_ptr<IScene> m_currentScene;

public:
    SceneChanger();
    ~SceneChanger();

    // シーンを変更するメソッド
    void ChangeScene(SceneType nextScene);

    // 現在のシーンの更新と描画を呼ぶ
    void Update();
    void Draw();
};