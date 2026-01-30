#pragma once
#include "DxLib.h"

class Character {
public:
    Character();
    void Initialize();
    void Update();
    void Draw() const;

private:
    int m_playerX;
    int m_playerY;
    int m_playerGraph;
};