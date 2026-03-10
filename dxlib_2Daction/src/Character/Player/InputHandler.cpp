#include "InputHandler.h"

InputHandler::InputHandler()
    : m_isRightPressed(false)
    , m_isLeftPressed(false)
    , m_isUpPressed(false)
    , m_isDownPressed(false)
    , m_prevUpPressed(false)
    , m_prevDownPressed(false)
{
}

void InputHandler::Update() {
    m_prevUpPressed = m_isUpPressed;
    m_prevDownPressed = m_isDownPressed;

    int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

    m_isRightPressed = (key & PAD_INPUT_RIGHT) != 0;
    m_isLeftPressed = (key & PAD_INPUT_LEFT) != 0;
    m_isUpPressed = (key & PAD_INPUT_UP) != 0;
    m_isDownPressed = (key & PAD_INPUT_DOWN) != 0;
}