#pragma once

class Camera {
public:
	Camera();
	~Camera();
	void Initialize(int screenWight, int screenHight);
	void Update(float targetX, float targetY);

	float GetX() const;
	float GetY() const;
private:
	float m_cameraX;
	float m_cameraY;
	int m_screenWidth;
	int m_screenHeight;
};