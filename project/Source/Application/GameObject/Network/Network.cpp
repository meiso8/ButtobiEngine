//
//#define CPPHTTPLIB_OPENSSL_SUPPORT // 必要に応じて
//#include "httplib.h"
//#include <thread>
#include "Network.h"

void Network::Initialize()
{
    //isTicketAcquired_ = false;
}

void Network::Update()
{
    //std::thread([]() {
    //    httplib::Server svr;

    //    // CORSヘッダーを付与（Netlifyからのクロスドメイン通信を許可するため必須）
    //    svr.set_default_headers({
    //        {"Access-Control-Allow-Origin", "*"}, // テスト用。本番はNetlifyのドメインを指定
    //        {"Access-Control-Allow-Methods", "POST, GET, OPTIONS"},
    //        {"Access-Control-Allow-Headers", "Content-Type"}
    //        });

    //    // プリフライトリクエスト (OPTIONS) のハンドリング
    //    svr.Options(R"(/.*)", [](const httplib::Request&, httplib::Response& res) {
    //        res.status = 200;
    //        });

    //    // 購入APIのエンドポイント
    //    svr.Post("/api/purchase", [](const httplib::Request& req, httplib::Response& res) {
    //        // チケット取得フラグを true に設定！
    //        isTicketAcquired_ = true;

    //        res.set_content(R"({"status":"success", "message":"Item unlocked!"})", "application/json");
    //        });

    //    // ポート 8080 でローカル待機
    //    svr.listen("127.0.0.1", 8080);
    //    }).detach();

}
