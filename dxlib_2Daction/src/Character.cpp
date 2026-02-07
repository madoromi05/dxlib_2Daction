#pragma once
#include "Character.h"
#include <stdexcept>
#include <string>
#include "map/Map.h"

Character::Character()
	: m_playerX(320), m_playerY(240), m_playerGraph(-1) {
}

void Character::Initialize() {
	m_playerGraph = LoadGraph(ResourcePath::Player::PLAYER_IMAGE);

	if (m_playerGraph == -1) {
		std::string errorMsg = "画像の読み込みに失敗しました。\nファイル名: Beryl.png\n";
		throw std::runtime_error(errorMsg);
	}

	// キャラクターの初期位置をセット
	m_playerX = 0;
	m_playerY = 0;
}

void Character::Update(Map* map) {
	// 移動しようとする予定地を今の場所で初期化
	int nextX = m_playerX;
	int nextY = m_playerY;
	// キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (Key & PAD_INPUT_UP)    nextY -= 3;
	if (Key & PAD_INPUT_DOWN)  nextY += 3;
	if (Key & PAD_INPUT_RIGHT) nextX += 3;
	if (Key & PAD_INPUT_LEFT)  nextX -= 3;

	if (map != nullptr) {
		if (!map->IsWall(nextX, nextY)) {
			m_playerX = nextX;
			m_playerY = nextY;
		}
	}
	else {
		// マップがない場合は無条件で移動
		m_playerX = nextX;
		m_playerY = nextY;
	}
}

void Character::Draw() const {
	// DrawGraph(m_playerX, m_playerY, m_playerGraph, TRUE);
}