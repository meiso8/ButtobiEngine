#include "LevelEditor.h"
#include "nlohmann/json.hpp"
void LevelEditor::Load(const std::string& fileName)
{
    // =============================JSONファイルを読み込んでみる=============================

    const std::string fullpath = kDefaultBaseFirectory + fileName + kExtension;

    std::ifstream file;

    file.open(fullpath);

    if (file.fail()) {
        assert(0);
    }

    // =============================ファイルチェック========================================

    nlohmann::json deserialized;

    file >> deserialized;

    assert(deserialized.is_object());
    assert(deserialized.contains("name"));
    assert(deserialized["name"].is_string());

    //"name"を文字列として取得
    std::string name =
        deserialized["name"].get<std::string>();
    //正しいレベルデータファイルかチェック
    assert(name.compare("scene") == 0);

    // =============================オブジェクト走査========================================

    LevelData* levelData = new LevelData();

    //"objects"の全オブジェクトを走査
    for (nlohmann::json& object : deserialized["objects"]) {
        assert(object.contains("type"));

        //種別を取得
        std::string type = object["type"].get<std::string>();
        //種類ごとの処理

        //MESHがある場合
        if (type.compare("MESH") == 0) {
            //要素追加
            levelData->objects.emplace_back(LevelData::ObjectData{});
            LevelData::ObjectData& objectData = levelData->objects.back();

            objectData.obj = std::make_unique<Object3d>();
            objectData.obj->Create();

            if (object.contains("file_name")) {
                objectData.fileName = object["file_name"];
            }
            //トランスフォームのパラメータ読み込み

            nlohmann::json& transform = object["transform"];
            //それぞれ座標系を合わせるため、yzの入れ替えを行っている
            //平行移動
            objectData.obj->worldTransform_.translate_.x = (float)transform["translation"][0];
            objectData.obj->worldTransform_.translate_.y = (float)transform["translation"][2];
            objectData.obj->worldTransform_.translate_.z = (float)transform["translation"][1];
            //回転角 軸回転方向を変換しておく
            objectData.obj->worldTransform_.rotate_.x = -(float)transform["rotation"][0];
            objectData.obj->worldTransform_.rotate_.y = -(float)transform["rotation"][2];
            objectData.obj->worldTransform_.rotate_.z = -(float)transform["rotation"][1];
            //スケーリング
            objectData.obj->worldTransform_.scale_.x = (float)transform["scaling"][0];
            objectData.obj->worldTransform_.scale_.y = (float)transform["scaling"][2];
            objectData.obj->worldTransform_.scale_.z = (float)transform["scaling"][1];

            //コライダーの読み込み
            objectData.collider = std::make_unique<Collider>();

            nlohmann::json& collider = object["collider"];
            objectData.collider->SetCenter(
                {
                    (float)collider["center"][0],
                    (float)collider["center"][2],
                    (float)collider["center"][1],
                }
                );
            if (collider["type"] == "BOX") {
                Vector3 size = { .x = (float)collider["size"][0] ,.y = (float)collider["size"][2] ,.z = (float)collider["size"][1] };
                size * 0.5f;
                objectData.collider->SetAABB({ .min = -size ,.max = size });
            } else {
                //一旦一番上の値を代入する
                objectData.collider->SetRadius((float)collider["size"][0] * 0.5f);
            }


        }



        //再帰処理
    }

}
