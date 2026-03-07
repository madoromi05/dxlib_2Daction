#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "IDrawable.h"

class DrawableList
{
private:
    // ソート用の構造体
    struct DrawRequest {
        int layer;
        IDrawable* drawable;

        // ソートのルール（layerが小さい順に並べる）
        bool operator<(const DrawRequest& other) const {
            return layer < other.layer;
        }
    };

    std::vector<DrawRequest> m_drawables;

public:
    DrawableList() = default;
    ~DrawableList() = default;

public:
    // 毎フレームのUpdate/Draw時にこれを呼んでリストに登録する
    void Add(IDrawable* drawable, int layer)
    {
        if (drawable && drawable->IsVisible()) {
            m_drawables.push_back({ layer, drawable });
        }
    }

    // 最後に1回だけ呼び出し、一気に描いてリストをリセットする
    void DrawAll(float cameraX, float cameraY)
    {
        // レイヤー順にソート
        std::sort(m_drawables.begin(), m_drawables.end());

        // 描画
        for (const auto& req : m_drawables) {
            req.drawable->Draw(cameraX, cameraY);
        }

        // 次のフレームのためにリストを空にする（メモリの確保領域は維持されるので高速）
        m_drawables.clear();
    }
};