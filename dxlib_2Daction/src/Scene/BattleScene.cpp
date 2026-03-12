#include "BattleScene.h"
#include "SceneChanger.h"
#include "DxLib.h"
#include <random>

BattleScene::BattleScene()
  : m_player(nullptr)
  , m_map(nullptr)
{}

BattleScene::~BattleScene() {
    Finalize();
}

void BattleScene::Initialize() {
    m_map = std::make_unique<Map>();

	std::random_device rd;
	unsigned int seed = rd();
    m_map->Initialize(seed);

    m_player = std::make_unique<PlayerCharacter>();
    m_player->Initialize(m_map.get());

    int monitorW = GetSystemMetrics(SM_CXSCREEN);
    int monitorH = GetSystemMetrics(SM_CYSCREEN);
    float aspect = static_cast<float>(monitorW) / static_cast<float>(monitorH);

    int cameraW = static_cast<int>(stage_information::kCameraHeight * aspect);
    int cameraH = stage_information::kCameraHeight;

    m_camera.Initialize(cameraW, cameraH);
}

void BattleScene::Update(SceneChanger* sceneChanger) {
    if (m_player != nullptr) {
        m_player->Update(m_map.get());
        m_camera.Update(
            static_cast<float>(m_player->GetX()),
            static_cast<float>(m_player->GetY())
        );
    }
    BuildDrawList();
}

void BattleScene::BuildDrawList() {
    if (m_map != nullptr) {
        m_map->RegisterTo(&m_drawableList);
    }

    if (m_player != nullptr) {
        int playerZOrder = stage_information::kZOrderObjectBase + m_player->GetY();
        m_drawableList.Add(m_player.get(), playerZOrder);
    }
}

void BattleScene::Draw() {
    m_drawableList.DrawAll(m_camera.GetX(), m_camera.GetY());
}

void BattleScene::Finalize() {
    m_player.reset();
    m_map.reset();
}
