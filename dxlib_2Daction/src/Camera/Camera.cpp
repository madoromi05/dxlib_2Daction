#include "Camera.h"
#include "map/StageInformation.h"
#include <algorithm>

Camera::Camera()
	: m_cameraX(0)
	, m_screenWidth(0)
	, m_screenHeight(0)
{}

Camera::~Camera() {}

void Camera::Initialize(int screenWidth, int screenHeight) {
	m_screenHeight = screenHeight;
	m_screenWidth = screenWidth;
}

void Camera::Update(float targetX, float targetY) {
	// カメラの中心をターゲットに合わせる
	m_cameraX = targetX - m_screenWidth / 2.0f;

	// ステージの範囲内にカメラが収まるように制限する
	float maxX = static_cast<float>(stage_information::kStagePixelWidth - m_screenWidth);
	float maxY = static_cast<float>(stage_information::kStagePixelHeight - m_screenHeight);

	m_cameraY = maxY;
	m_cameraX = std::max(0.0f, std::min(m_cameraX, maxX));
}

float Camera::GetX() const { return m_cameraX; }
float Camera::GetY() const { return m_cameraY; }