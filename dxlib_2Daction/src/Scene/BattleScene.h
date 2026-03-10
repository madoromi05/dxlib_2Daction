#pragma once
#include "IScene.h"
#include "Character/Player/PlayerCharacter.h"
#include "Camera/Camera.h"
#include "DrawInterface/DrawableList.h"
#include "map/Map.h"
#include "map/StageInformation.h"
#include <memory>

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
    std::unique_ptr<PlayerCharacter> m_player;
    std::unique_ptr<Map> m_map;
    DrawableList m_drawableList;
    Camera m_camera;
};