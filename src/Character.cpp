#pragma once
#include "Character.h"

Character::Character()
	: m_playerX(320), m_playerY(240), m_playerGraph(-1) {
}

void Character::Initialize() {
	m_playerGraph = LoadGraph("Beryl.png");

	if (m_playerGraph == -1) {
		MessageBox(NULL, _T("画像の読み込みに失敗しました。\nファイル名と場所を確認してください。"), _T("エラー"), MB_OK);
		DxLib_End();
		exit(-1);
	}
	// キャラクターの初期位置をセット
	m_playerX = 0;
	m_playerY = 0;
}

void Character::Update() {
	// キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 上を押していたら上に進む
	if (Key & PAD_INPUT_UP) m_playerY -= 3;

	// 下を押していたら下に進む
	if (Key & PAD_INPUT_DOWN) m_playerY += 3;

	// 右を押していたら右に進む
	if (Key & PAD_INPUT_RIGHT) m_playerX += 3;

	// 左を押していたら左に進む
	if (Key & PAD_INPUT_LEFT) m_playerX -= 3;
}

void Character::Draw() const {
	DrawGraph(m_playerX, m_playerY, m_playerGraph, TRUE);
}