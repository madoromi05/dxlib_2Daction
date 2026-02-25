#pragma once
#include "DrowInterface/DrawableList.h"
#include "Collider/Collider.h"
#include <vector>

class Map;

enum class ColliderType {
    Body = 0, // 地形との当たり判定用
    Attack,   // 敵への攻撃判定用（例）
    Damage,   // 敵からダメージを受ける判定用（例）
    Count     // 種類の総数
};

class Character : public IDrawable {
public:
    Character();
    void Initialize();
    void Update(class Map* map);
    void Draw() const override;

    int GetY() const { return m_playerY; }
    int m_playerSpeed = 3;

private:
    void Move(class Map* map, int key);
    void ResetJumpParam();
    void JumpMove(class Map* map, bool jumpBtnPress);
    void JumpCheck(bool jumpBtnPress);
    void PreparingJump();
    void MoveProcess(class Map* map, bool jumpBtnPress);
    void Physics(class Map* map);

    float m_playerX;
    float m_playerY;

    std::vector<Collider> m_colliders;
    bool m_isDebugMode;

    //重力
    static constexpr float kGravity = 0.98f;
  
    // ジャンプ
    int m_verticalPositionOrigin;	// ジャンプ開始時の位置
    int m_verticalSpeed;			// 速度
    int m_verticalForce;			// 現在の加速度
    int m_verticalForceFall;		// 降下時の加速度
    int m_verticalForceDecimalPart;	// 加速度の増加値
    int m_correctionValue;			// 累積計算での補正値
    int m_horizontalSpeed = 00;		// 横方向速度

    bool m_jumpBtnPrevPress;            // 1フレーム前のジャンプボタン押下状態
    bool m_isGrounded;                  // 地面にいるか
    
    // ジャンプ開始時の初期パラメータ(速度によって変わる)
    static constexpr int kVerticalForceDecimalPartData[5] = { 40, 40, 30, 60, 60 };  // 上昇中の重力
    static constexpr int kVerticalFallForceData[5] = { 100, 100, 80, 140, 140 };     // 落下中・小ジャンプ時の重力
    static constexpr int kInitialVerticalSpeedData[5] = { -7, -7, -8, -10, -12 };    // ジャンプの初速度
    static constexpr int kInitialVerticalForceData[5] = { 0, 0, 0, 0, 0 };           // 初期加速度

    // 落下速度の最大値
    static constexpr int kDownSpeedLimit = 1000;
    
    // アニメーション定数（画像に合わせて調整してください）
    static constexpr int kIdleFrameCount = 6;  // 画像のコマ数
    static constexpr int kAnimSpeed = 8;       // アニメーションの速度（大きいほど遅い）

    // 画像ハンドル配列（コマ数分用意する）
    int m_idleHandles[kIdleFrameCount];

    // アニメーション管理用
    int m_animTimer;      // 時間計測用
    int m_currentFrame;   // 現在表示しているコマ番号(0 ~ 5)

    static constexpr int kChipWidth = 128;
    static constexpr int kChipHeight = 128;
    static constexpr int kDrawWidth = 128;
    static constexpr int kDrawHeight = 128;
};