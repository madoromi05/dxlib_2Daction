#pragma once
#include "IScene.h"
#include "Character.h"
#include "map/Map.h"

class BattleScene : public IScene {
private:
    Character* _player;
	Map* _map;
    
public:
    BattleScene();
    virtual ~BattleScene();

    void Initialize() override;
    void Update(SceneChanger* sceneChanger) override;
    void Draw() override;
    void Finalize() override;
};