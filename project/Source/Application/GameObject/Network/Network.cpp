
#include "Network.h"
//HTTPS通信を行うためのサポート
#define CPPHTTPLIB_OPENSSL_SUPPORT 
#include "httplib.h"
#include <iostream>

#include <thread>
#include<Log.h>
#include <filesystem>

Network::Network()
{
}

Network::~Network()
{
}

void Network::Initialize()
{
    isTicketGet_ = false;
}


void Network::OnButtonClicked()
{

    std::thread([this]() {

        httplib::Server srv;


        std::string mountPath = "";
        if (std::filesystem::exists("Source/webSite")) {
            mountPath = "Source/webSite"; // GE.slnと同じ階層が基準の場合
        } else if (std::filesystem::exists("webSite")) {
            mountPath = "webSite"; // Sourceフォルダ内が基準の場合
        } else if (std::filesystem::exists("../../Source/webSite")) {
            mountPath = "../../Source/webSite"; // x64/Debugフォルダなどから直接起動した場合
        }

        if (mountPath.empty()) {
            LogFile::Log("エラー: webSiteフォルダが見つかりません。");
            return; // フォルダがない場合はサーバーを起動しない
        }

        // 見つかったパスをマウント
        srv.set_mount_point("/", mountPath.c_str());

   
        srv.Post("/api/purchase", [this](const httplib::Request& req, httplib::Response& res) {
            isTicketGet_ = true;
            LogFile::Log("チケット購入完了");
            res.set_content("{\"status\":\"success\"}", "application/json");
            });


        std::cout << "サーバー起動: http://127.0.0.1:8080" << std::endl;
        std::cout << "ブラウザで http://127.0.0.1:8080/confirm.html を開いてください" << std::endl;

        srv.listen("127.0.0.1", 8080);

        }).detach();

    ShellExecuteA(NULL, "open", "http://127.0.0.1:8080/index.html", NULL, NULL, SW_SHOWNORMAL);
}
