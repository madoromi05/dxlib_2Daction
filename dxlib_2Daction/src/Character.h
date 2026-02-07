#pragma once
#include "DxLib.h"
#include "ResorcePath.h"
class Map;

class Character {
public:
    Character();
    void Initialize();
    void Update(Map* map);
    void Draw() const;

private:
    int m_playerX;
    int m_playerY;
    int m_playerGraph;
};