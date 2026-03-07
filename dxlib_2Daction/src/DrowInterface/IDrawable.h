#pragma once

// 描画可能なオブジェクトのインターフェース。

class IDrawable
{
private:
    bool m_isVisible;

public:
    IDrawable() : m_isVisible(true) {};
    virtual ~IDrawable() = default;

public:
    virtual void Draw(float cameraX, float cameraY) const = 0;

    // オブジェクトを「表示する」ように設定
    void Show()
    {
        m_isVisible = true;
    }

    // オブジェクトを「表示する」ように設定
    void Hide()
    {
        m_isVisible = false;
    }

	// オブジェクトが「表示する」状態かどうかを返す
    bool IsVisible() const
    {
        return m_isVisible;
    }
};
