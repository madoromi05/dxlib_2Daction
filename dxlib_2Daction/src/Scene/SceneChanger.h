#pragma once
#include "IScene.h"

class SceneChanger {
private:
    IScene* m_currentScene;

public:
    SceneChanger();
    ~SceneChanger();

    // シーンを変更するメソッド
    void ChangeScene(SceneType nextScene);

    // 現在のシーンの更新と描画を呼ぶ
    void Update();
    void Draw();
};