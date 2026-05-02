#include "LevelEditor.h"
#include"Model.h"


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

    levelData_ = std::make_unique<LevelData>();

    levelData_->objects.reserve(kMaxObjectCount_);

    //"objects"の全オブジェクトを走査
    for (nlohmann::json& object : deserialized["objects"]) {

        LoadObject(object, levelData_.get());
    }

}

void LevelEditor::CreateObject(std::vector<std::unique_ptr<ObjectSet>>& objects)
{
    for (auto& objectData : levelData_->objects) {

        if (objectData.disabled) {
            continue;
        }

        std::unique_ptr<ObjectSet> newObjctData = std::make_unique<ObjectSet>();

        newObjctData->obj_ = std::make_unique<Object3d>();
        newObjctData->obj_->Create();
        auto& transform = newObjctData->obj_->worldTransform_;
        transform.translate_ = objectData.transform.translate;
        transform.scale_ = objectData.transform.scale;
        transform.rotate_ = objectData.transform.rotate;
        Model* model = ModelManager::GetModel(objectData.fileName);
        newObjctData->obj_->SetMesh(model);

        //コライダーの設定
        newObjctData->collider_ = std::make_unique<Collider>();
        newObjctData->collider_->SetCenter(objectData.colliderData.center);
        Vector3 size = objectData.colliderData.size;
        size *= 0.5f;
        newObjctData->collider_->SetAABB({ .min = -size ,.max = size });

        objects.push_back(std::move(newObjctData));
    }
}

void LevelEditor::LoadObject(nlohmann::json& object, LevelData* levelData) {

    assert(object.contains("type"));
    //種別を取得
    std::string type = object["type"].get<std::string>();
    //種類ごとの処理



    //MESHがある場合
    if (type.compare("MESH") == 0) {
        //要素追加
        levelData->objects.emplace_back(LevelData::ObjectData{});
        LevelData::ObjectData& objectData = levelData->objects.back();

        if (object.contains("file_name")) {
            objectData.fileName = object["file_name"];
        }
        //トランスフォームのパラメータ読み込み

        nlohmann::json& transform = object["transform"];
        //それぞれ座標系を合わせるため、yzの入れ替えを行っている
        //平行移動
        objectData.transform.translate.x = (float)transform["translation"][0];
        objectData.transform.translate.y = (float)transform["translation"][2];
        objectData.transform.translate.z = (float)transform["translation"][1];
        //回転角 軸回転方向を変換しておく
        objectData.transform.rotate.x = -(float)transform["rotation"][0];
        objectData.transform.rotate.y = -(float)transform["rotation"][2];
        objectData.transform.rotate.z = -(float)transform["rotation"][1];
        //スケーリング
        objectData.transform.scale.x = (float)transform["scaling"][0];
        objectData.transform.scale.y = (float)transform["scaling"][2];
        objectData.transform.scale.z = (float)transform["scaling"][1];

        //コライダーの読み込み
        if (object.contains("collider")) {

            nlohmann::json& collider = object["collider"];

            objectData.colliderData.center = {
                    (float)collider["center"][0],
                    (float)collider["center"][2],
                    (float)collider["center"][1],
            };
            objectData.colliderData.size = { 
                .x = (float)collider["size"][0] ,
                .y = (float)collider["size"][2] ,
                .z = (float)collider["size"][1] 
            };
          
        }

        //無効かどうかのフラグ
        if (object.contains("disabled")) {
            objectData.disabled = object["disabled"];
        }

        //オブジェクト走査を再起関数にまとめ、再帰呼び出して枝を走査する
        if (object.contains("children")) {

            for (nlohmann::json& child : object["children"]) {
                LoadObject(child, levelData);
            }
        }
    }
}
