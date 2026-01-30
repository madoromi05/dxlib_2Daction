#pragma once

// main関数から呼ばれるエントリーポイント
void Run();

class GameAdmin {
public:
    GameAdmin();
    ~GameAdmin();
    void Initialize();
    void MainLoop();

private:
    // キャラクターなどのオブジェクトを保持
    // 以前の命名規則に基づき、クラスのインスタンスを定義
    class Character* m_pPlayer;
};