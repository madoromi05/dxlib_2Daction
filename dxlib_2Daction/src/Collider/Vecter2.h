#pragma once
#include <cmath>

/// <summary>
/// 2Dベクトル構造体。
/// 衝突判定で使用する内積・外積・回転・正規化などの演算を提供する。
/// </summary>
struct Vec2 {
    float m_x;
    float m_y;

    Vec2() : m_x(0.0f), m_y(0.0f) {}
    Vec2(float x, float y) : m_x(x), m_y(y) {}

    // 四則演算
    Vec2 operator+(const Vec2& other) const { return Vec2(m_x + other.m_x, m_y + other.m_y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(m_x - other.m_x, m_y - other.m_y); }
    Vec2 operator*(float scalar) const { return Vec2(m_x * scalar, m_y * scalar); }
    Vec2 operator/(float scalar) const { return Vec2(m_x / scalar, m_y / scalar); }

    Vec2& operator+=(const Vec2& other) { m_x += other.m_x; m_y += other.m_y; return *this; }
    Vec2& operator-=(const Vec2& other) { m_x -= other.m_x; m_y -= other.m_y; return *this; }

    Vec2 operator-() const { return Vec2(-m_x, -m_y); }

    /// <summary>
    /// 内積
    /// 分離軸定理で射影の計算に使用する
    /// </summary>
    static float Dot(const Vec2& a, const Vec2& b) {
        return a.m_x * b.m_x + a.m_y * b.m_y;
    }

    /// <summary>
    /// 外積
    /// 2Dでは符号で左右どちら側にあるかを判定できる
    /// </summary>
    static float Cross(const Vec2& a, const Vec2& b) {
        return a.m_x * b.m_y - a.m_y * b.m_x;
    }

    /// <summary>
    /// ベクトルの長さ
    /// </summary>
    float Length() const {
        return std::sqrt(m_x * m_x + m_y * m_y);
    }

    /// <summary>
    /// ベクトルの長さの二乗（平方根を避けて高速比較するために使用）
    /// </summary>
    float LengthSquared() const {
        return m_x * m_x + m_y * m_y;
    }

    /// <summary>
    /// 正規化（長さ1のベクトルにする）
    /// 分離軸を求めるときに使用する
    /// </summary>
    Vec2 Normalized() const {
        float len = Length();
        if (len < 1e-6f) return Vec2(0.0f, 0.0f);
        return Vec2(m_x / len, m_y / len);
    }

    /// <summary>
    /// 法線ベクトル（90度回転）
    /// 辺に対する分離軸を求めるために使用する
    /// </summary>
    Vec2 Perpendicular() const {
        return Vec2(m_y, -m_x);
    }

    /// <summary>
    /// 任意角度で回転（ラジアン）
    /// OBBの頂点計算やコライダーの左右反転に使用する
    /// 回転行列: [cos -sin] [x]
    ///           [sin  cos] [y]
    /// </summary>
    Vec2 Rotated(float radians) const {
        float cosR = std::cos(radians);
        float sinR = std::sin(radians);
        return Vec2(
            m_x * cosR - m_y * sinR,
            m_x * sinR + m_y * cosR
        );
    }

    /// <summary>
    /// X軸方向に反転（左右反転用）
    /// キャラクターが左を向いたとき、コライダーのオフセットを反転する
    /// </summary>
    Vec2 FlippedX(float pivotX) const {
        return Vec2(2.0f * pivotX - m_x, m_y);
    }
};