#pragma once
#include "DrawInterface/DrawableList.h"
#include "Collider/Collider.h"
#include "SpriteAnimation.h"
#include <vector>

class Map;

enum class AnimState {
    Idle,       // 待機
    Run,        // 走り
    JumpUp,     // ジャンプ上昇中
    JumpDown,   // 落下中
    Attack,     // 攻撃
    Count       // 状態の総数（最後に配置）
};

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
    void Draw(float cameraX, float cameraY) const override;
    int GetY() const { return m_playerY; }
    int GetX() const { return m_playerX; }

private:
    void Move(class Map* map, int key);                         // Playerの横入力による移動
	void ResetJumpParam();                                      // ジャンプから地面にいる状態にする
	void JumpMove(class Map* map, bool jumpBtnPress);           // ジャンプと重力の処理
	void JumpCheck(bool jumpBtnPress);                          // ジャンプの条件チェック
	void PreparingJump();                                       // ジャンプ開始の準備
	void MoveProcess(class Map* map, bool jumpBtnPress);        // ジャンプと重力の処理
	void Physics(class Map* map);                               // ジャンプと重力の物理計算

    SpriteAnimation m_idleAnimation;

    float m_playerX;
    float m_playerY;

    int m_playerSpeed = 2;

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

    bool m_jumpBtnPrevPress;        // 1フレーム前のジャンプボタン押下状態
    bool m_isGrounded;              // 地面にいるか
	bool m_isJumping;               // ジャンプ中かどうか
	bool m_isFacingLeft;            // 左向きかどうか

    // ジャンプ開始時の初期パラメータ
	static constexpr int kVerticalForceDecimalPartData[5] = { 20, 20, 30, 30, 50 };     // ジャンプ中の加速度
    static constexpr int kVerticalFallForceData[5]        = { 40, 40, 60, 80, 100 };    // 落下中重力加速度
    static constexpr int kInitialVerticalSpeedData[5]     = { -6, -6, -7, -8, -10 };    // ジャンプの初速度
    static constexpr int kInitialVerticalForceData[5]     = { 0, 0, 0, 0, 0 };          // 初期加速度

    static constexpr int kDownSpeedLimit = 100;    // 落下速度の最大値
	static constexpr int kNormalGravity  = 150;    //非Jump中の重力

	// キャラクターの描画サイズ
    static constexpr int kChipWidth  = 128;
    static constexpr int kChipHeight = 128;
    static constexpr int kDrawWidth  = 128;
    static constexpr int kDrawHeight = 128;

    // 攻撃用
    bool m_attackBtnPrevPress;

	// アニメーション管理用
    SpriteAnimation m_animations[static_cast<int>(AnimState::Count)];
    SpriteAnimation* m_currentAnimation;
    AnimState m_currentState;
    void ChangeAnimation(AnimState newState);
};