#pragma once
#include <string>
#include <vector>

class SpriteAnimation {
public:
    SpriteAnimation();
    ~SpriteAnimation();

    // 画像の読み込み
    void Load(const std::string& filePath,
		int totalFrameCount,    // 読み込むコマ数 
		int splitX, int splitY, // 画像を分割する数
		int frameWidth,         // コマの幅ピクセル
		int frameHeight);       // コマの高さピクセル
    void Update();  
    void Draw(float x, float y, int drawWidth, int drawHeight, bool isTurn = false) const;
    void SetAnimSpeed(int speed);
	void Reset();                   // アニメーションを最初のコマにリセット
	void SetLoop(bool isLoop);      // ループするかしないか設定
    bool IsFinished() const;

private:
    std::vector<int> m_handles;                 // 分割読み込みした画像のハンドル群
    int m_animTimer;                            // 時間計測用タイマー
    int m_currentFrame;                         // 現在表示しているコマ番号
	int m_animSpeed;                            // アニメーションの速度
    int m_frameCount;                           // 総コマ数

    bool m_isLoop;    // ループフラグ
    bool m_isFinished;// 終了フラグ
};