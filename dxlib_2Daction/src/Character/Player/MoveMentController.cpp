#include "MovementController.h"
#include "map/Map.h"
#include "Collider/MapCollider.h"

MovementController::MovementController()
    : m_posX(0)
    , m_posY(0)
    , m_playerSpeed(2)
    , m_horizontalSpeed(0)
    , m_isFacingLeft(false)
    , m_gravityDirection(1)
    , m_verticalSpeed(0)
    , m_isGrounded(false)
    , m_verticalForceDecimalPart(0)
    , m_bodyCollider(60.0f, 90.0f, 34.0f, 40.0f)
{}

void MovementController::Initialize() {
    m_posX = 0;
    m_posY = 475;
    m_isFacingLeft = false;
    m_gravityDirection = 1;
    m_verticalSpeed = 0;
    m_verticalForceDecimalPart = 0;
    m_isGrounded = false;

    UpdateColliderPositions();
}

void MovementController::UpdateColliderPositions() {
    m_bodyCollider.SetWorldPosition(m_posX, m_posY, m_isFacingLeft, kSpriteWidth);
}

bool MovementController::IsCollidingWithMap(Map* map, float nextX, float nextY) {
    return MapCollider::IsCollidingWithMap(
        m_bodyCollider, map, nextX, nextY, m_isFacingLeft, kSpriteWidth);
}

void MovementController::SetGravityDirection(int direction) {
    if (m_gravityDirection == direction) return;

    m_gravityDirection = direction;
    m_isGrounded = false;
    m_verticalForceDecimalPart = 0;
}

// ===================================================
// 横方向の移動
// ===================================================
void MovementController::MoveHorizontal(Map* map, bool isRightPressed, bool isLeftPressed) {
    float nextX = m_posX;

    if (isRightPressed) {
        nextX += m_playerSpeed;
        m_horizontalSpeed = m_playerSpeed;
        m_isFacingLeft = false;
    }else if (isLeftPressed) {
        nextX -= m_playerSpeed;
        m_horizontalSpeed = -m_playerSpeed;
        m_isFacingLeft = true;
    }else {
        m_horizontalSpeed = 0;
    }

    if (map != nullptr) {
        if (!IsCollidingWithMap(map, nextX, m_posY)) {
            m_posX = nextX;
        }
        else {
            float stepHeight = static_cast<float>(m_playerSpeed) + 2.0f;
            bool canStepUp = false;
            float stepDir = static_cast<float>(-m_gravityDirection);

            for (float s = 2.0f; s <= stepHeight; s += 2.0f) {
                if (!IsCollidingWithMap(map, nextX, m_posY + stepDir * s)) {
                    m_posX = nextX;
                    m_posY += stepDir * s;
                    canStepUp = true;
                    break;
                }
            }

            if (!canStepUp) {
                float step = (nextX > m_posX) ? 1.0f : -1.0f;
                while (!IsCollidingWithMap(map, m_posX + step, m_posY)) {
                    m_posX += step;
                }
            }
        }
    }
    else {
        m_posX = nextX;
    }
}

void MovementController::UpdatePhysics(Map* map) {
    Physics(map);
}

void MovementController::Physics(Map* map) {
    float groundCheckOffset = static_cast<float>(m_gravityDirection);

    if (m_isGrounded && map != nullptr) {
        if (!IsCollidingWithMap(map, m_posX, m_posY + groundCheckOffset)) {
            m_isGrounded = false;
        }
    }

    if (m_isGrounded) {
        m_verticalSpeed = 0;
        m_verticalForceDecimalPart = 0;
        return;
    }

    // 固定小数点の重力加速
    int gravity = kGravityAccel * m_gravityDirection;
    m_verticalForceDecimalPart += gravity;

    while (m_verticalForceDecimalPart >= 256) {
        m_verticalForceDecimalPart -= 256;
        m_verticalSpeed++;
    }
    while (m_verticalForceDecimalPart <= -256) {
        m_verticalForceDecimalPart += 256;
        m_verticalSpeed--;
    }

    if (m_verticalSpeed > kMaxFallSpeed) {
        m_verticalSpeed = kMaxFallSpeed;
        m_verticalForceDecimalPart = 0;
    }
    if (m_verticalSpeed < -kMaxFallSpeed) {
        m_verticalSpeed = -kMaxFallSpeed;
        m_verticalForceDecimalPart = 0;
    }

    float nextY = m_posY + m_verticalSpeed;

    if (map != nullptr) {
        if (!IsCollidingWithMap(map, m_posX, nextY)) {
            m_posY = nextY;
        }
        else {
            while (!IsCollidingWithMap(map, m_posX, m_posY + groundCheckOffset)) {
                m_posY += groundCheckOffset;
            }
            m_verticalSpeed = 0;
            m_verticalForceDecimalPart = 0;
            m_isGrounded = true;
        }
    }
    else {
        m_posY = nextY;
    }
}