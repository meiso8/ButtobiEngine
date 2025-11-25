#include"../Game.h"

// Windowsアプリでのエントリーポイント(main関数)

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->Initialize();
    // =============================================
    // ウィンドウのxボタンが押されるまでループ メインループ
    // =============================================
    while (true) {

        // メッセージが来たらループを抜ける
        if (Window::ProcessMassage()) {
            break;
        }

        game->Update();

        //エスケープボタンを押したら終了
        if (Input::IsTriggerKey(DIK_ESCAPE)) { break; }

        game->Debug();

        game->Draw();

    }

    game->Finalize();

    return 0;
}
