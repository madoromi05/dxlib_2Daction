#pragma once

// シーンの種類を定義
enum class SceneType {
    Title,
    Battle,
    GameClear,
    GameOver
};

class SceneChanger;

// シーンのインターフェース
class IScene {
public:
    virtual ~IScene() = default;

    // 初期化
    virtual void Initialize() = 0;

    // 更新
    virtual void Update(SceneChanger* sceneChanger) = 0;

    // 描画
    virtual void Draw() = 0;

    // 終了処理
    virtual void Finalize() = 0;
};