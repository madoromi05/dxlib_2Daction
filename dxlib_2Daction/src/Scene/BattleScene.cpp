#include "BattleScene.h"
#include "SceneChanger.h"
#include "DxLib.h"

BattleScene::BattleScene() : _player(nullptr) {
    // コンストラクタは変数の初期化のみ
}

BattleScene::~BattleScene() {
    Finalize();
}

void BattleScene::Initialize() {
    _map = new Map();
    _map->Initialize();

    _player = new Character();
    _player->Initialize();
}

void BattleScene::Update(SceneChanger* sceneChanger) {
    if (_player != nullptr) {
        _player->Update(_map);
    }
}

void BattleScene::Draw() {

    if (_map != nullptr) {
        _map->DrawBack();
    }

    if (_player != nullptr) {
        _player->Draw();
    }

    if (_map != nullptr) {
        _map->DrawFront();
    }
}

void BattleScene::Finalize() {
    if (_player != nullptr) {
        delete _player;
        _player = nullptr;
    }
    if (_map != nullptr) {
        delete _map;
        _map = nullptr;
    }
}