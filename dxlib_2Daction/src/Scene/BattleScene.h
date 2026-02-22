#pragma once
#include "IScene.h"
#include "Player/Character.h"

class BattleScene : public IScene {
private:
    Character* m_player;
	Map* m_map;
    
public:
    BattleScene();
    virtual ~BattleScene();

    void Initialize() override;
    void Update(SceneChanger* sceneChanger) override;
    void Draw() override;
    void Finalize() override;
};