#pragma once
#include "IScene.h"
#include "Character.h"

class BattleScene : public IScene {
private:
    Character* _player;
public:
    BattleScene();
    virtual ~BattleScene();

    void Initialize() override;
    void Update(SceneChanger* sceneChanger) override;
    void Draw() override;
    void Finalize() override;
};