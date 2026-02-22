#include "BattleScene.h"
#include "map/Map.h"
#include "SceneChanger.h"
#include "DxLib.h"

BattleScene::BattleScene() : m_player(nullptr) {
    // コンストラクタは変数の初期化のみ
}

BattleScene::~BattleScene() {
    Finalize();
}

void BattleScene::Initialize() {
    m_map = new Map();
    m_map->Initialize();

    m_player = new Character();
    m_player->Initialize();
}

void BattleScene::Update(SceneChanger* sceneChanger) {
    if (m_player != nullptr) {
        m_player->Update(m_map);
    }
}

void BattleScene::Draw() {

    if (m_map != nullptr) {
        m_map->RegisterTo(&m_drawableList);
    }

    if (m_player != nullptr) {
        int playerZOrder = stage_information::kZOrderObjectBase + m_player->GetY();
        m_drawableList.Add(m_player, playerZOrder);
    }

    m_drawableList.DrawAll();
}

void BattleScene::Finalize() {
    if (m_player != nullptr) {
        delete m_player;
        m_player = nullptr;
    }
    if (m_map != nullptr) {
        delete m_map;
        m_map = nullptr;
    }
}