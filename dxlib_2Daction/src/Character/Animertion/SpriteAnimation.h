#pragma once
#include <string>
#include <vector>
#include <tchar.h>

typedef std::basic_string<TCHAR> tstring;

class SpriteAnimation {
public:
    SpriteAnimation();
    ~SpriteAnimation();

    // 画像の読み込み
    void Load(const tstring& filePath,
		int totalFrameCount,    // 読み込むコマ数 
		int splitX, int splitY, // 画像を分割する数
		int frameWidth,         // コマの幅ピクセル
		int frameHeight);       // コマの高さピクセル
    void Update();  
    void Draw(float x, float y, int drawWidth, int drawHeight, bool isTurn = false) const;
    
    // DrawModiGraph: 4頂点を指定して自由変形描画
    // (x1,y1)=左上, (x2,y2)=右上, (x3,y3)=右下, (x4,y4)=左下
    void DrawModified(int x1, int y1, int x2, int y2,
        int x3, int y3, int x4, int y4) const;
    void SetAnimSpeed(int speed);
	void Reset();                   // アニメーションを最初のコマにリセット
	void SetLoop(bool isLoop);      // ループするかしないか設定
    bool IsFinished() const;

private:
    std::vector<int> m_handles;     // 分割読み込みした画像のハンドル群
    int m_animTimer;                // 時間計測用タイマー
    int m_currentFrame;             // 現在表示しているコマ番号
	int m_animSpeed;                // アニメーションの速度
    int m_frameCount;               // 総コマ数

    bool m_isLoop;                  // ループフラグ
    bool m_isFinished;              // 終了フラグ
};
