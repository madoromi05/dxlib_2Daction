#pragma once
#include "IScene.h"
#include "Player/Character.h"
#include "Camera/Camera.h"

class BattleScene : public IScene {
public:
    BattleScene();
    virtual ~BattleScene();

    void Initialize() override;
    void Update(SceneChanger* sceneChanger) override;
    void Draw() override;
    void Finalize() override;

private:
    void BuildDrawList();
    std::unique_ptr<Character> m_player;
    std::unique_ptr<Map> m_map;
    DrawableList m_drawableList;
    Camera m_camera;
};