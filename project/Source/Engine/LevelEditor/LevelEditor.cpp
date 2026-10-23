#include "LevelEditor.h"
#include"Model.h"
#include<ctype.h>
#include"../Mesh/PrimitiveFactory/PrimitiveFactory.h"
#include"ObjectManager/ObjectManager.h"

const std::array<std::string, LevelEditor::kObjectTypeNames>  LevelEditor::objectTypeName_ = { "MESH","PlayerSpawn","EnemySpawn","StageChangeTrigger","Tree"};

void LevelEditor::Load(const std::string& fileName, bool useButtobiEditor)
{
    // =============================JSONLoad============================

    const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

    std::ifstream file;

    file.open(fullpath);

    if (file.fail()) {
        assert(0);
    }

    // =====================================================================

    nlohmann::json deserialized;

    file >> deserialized;

    assert(deserialized.is_object());
    assert(deserialized.contains("name"));
    assert(deserialized["name"].is_string());

    //nameを含むか
    std::string name =
        deserialized["name"].get<std::string>();
    //小文字にする
    std::string toLowerName;
    for (auto& w : name) {
        toLowerName += std::tolower(w);
    }

    //sceneがあればOK
    assert(toLowerName.find("scene") != std::string::npos);

    // =============================レベルデータの読み込み、エンジン側で調整しているかどうかを判定する=======================================

    levelData_ = std::make_unique<LevelData>();

    levelData_->objects.reserve(kMaxObjectCount_);

    if (useButtobiEditor) {
        vector3Name_ = { "x","y","z" };
        transformsName_ = { "translate","rotate","scale" };

    } else {
        vector3Name_ = { "x","z","y" };
        transformsName_ = { "translation","rotation","scaling" };
    }

    //"objects"を読み込む
    for (nlohmann::json& object : deserialized["objects"]) {
        //無効フラグが立っているときは飛ばす
        if (object.contains("disabled")) {
            bool disabled = object["disabled"].get<bool>();

            if (disabled) {
                continue;
            }
        }
        //オブジェクトのロード
        LoadObject(object, levelData_.get());
    }

}

void LevelEditor::CreateObject(std::vector<std::unique_ptr<ObjectSet>>& objects)
{

    objects.clear();

    for (auto& objectData : levelData_->objects) {

        std::unique_ptr<ObjectSet> newObjectData = std::make_unique<ObjectSet>();
        auto filePath = objectData.filePath;

        Model* model = nullptr;

        if (!filePath.directoryPath.empty()) {
            model = ModelManager::LoadModelAndGet(filePath.directoryPath + "/" + filePath.fileName);
        }

        newObjectData->obj_ = std::make_unique<Object3d>();
        newObjectData->obj_->Create();
        newObjectData->obj_->SetObjectType("MESH");

        if (model) {
            //モデルデータと一緒にテクスチャもセットされる
            newObjectData->obj_->SetMeshAndMaterial(model);
        } else {
            auto* primitive = PrimitiveFactory::GetPrimitiveForName(objectData.filePath.fileName);
            newObjectData->obj_->SetMeshAndMaterial(primitive);
            //テクスチャハンドルを入れる 

            if (!objectData.materialData.textureFilePath.empty()) {
                //テクスチャファイルパスを入れる。
                newObjectData->obj_->SetSRVHandle(Texture::AddTextureHandle(objectData.materialData.textureFilePath));

            }
       
        }

        newObjectData->obj_->SetColor(objectData.materialData.color);
        newObjectData->obj_->SetTemperature(objectData.materialData.temperature);
        newObjectData->obj_->SetShininess(objectData.materialData.shininess);
        //環境光
        newObjectData->obj_->SetEnvironmentCoefficient(objectData.materialData.environmentCoefficient);
        //ガラス度合い
        newObjectData->obj_->SetGlassFactor(objectData.materialData.glassFactor);
        //ライトモード
        newObjectData->obj_->SetLightMode(static_cast<Object3d::LightMode>(objectData.materialData.lightMode));

        newObjectData->obj_->SetObjectName(objectData.objectName);

        //ロックされているかの情報は受け渡さない

        newObjectData->obj_->RegisterObject();

        auto& transform = newObjectData->obj_->GetWorldTransform();

        transform.eTransform_ = objectData.transform;
        //ここで一度 Object3d の行列計算(Update)を走らせて matWorld_ を確定させる！
        newObjectData->obj_->Update();

        //コライダーの設定
        newObjectData->collider_ = std::make_unique<Collider>();
        ObjectManager::GetInstance()->RegisterCollider(newObjectData->collider_.get(), newObjectData->obj_.get());
        newObjectData->collider_->SetCenter(objectData.colliderData.center);


        Vector3 size = objectData.colliderData.size;
        size *= 0.5f;
        if (objectData.colliderData.type == "Sphere") {
            newObjectData->collider_->SetRadius(size.x);
        }

        if (objectData.colliderData.type == "AABB") {
            newObjectData->collider_->SetAABB({ .min = -size ,.max = size });
        }

        //属性の設定
        newObjectData->collider_->SetCollisionAttribute(CollisionTag::GetTag(objectData.colliderData.tag));
        newObjectData->collider_->SetCollisionMask(objectData.colliderData.maskTag);

        //コライダーに自分のワールドマトリックスのポインタを追加する
        newObjectData->collider_->SetWorldMatrix(newObjectData->obj_->GetWorldTransform().matWorld_);
        objects.push_back(std::move(newObjectData));
    }
}

void LevelEditor::CreateStageChangeTriggers(std::vector<std::unique_ptr<StageChangeTrigger>>& triggers)
{

    //ステージ遷移トリガーをクリアする
    triggers.clear();

    for (auto& triggerData : levelData_->stageChangeTriggers_) {

        // 生成
        std::unique_ptr<StageChangeTrigger> newTrigger = std::make_unique<StageChangeTrigger>();
        newTrigger->Create(triggerData);
        //データ挿入
        triggers.push_back(std::move(newTrigger));
    }

}

void LevelEditor::CreateObject3d(std::vector<std::unique_ptr<Object3d>>& object3ds, std::string objectType)
{
    object3ds.clear();

    for (auto& objectData : levelData_->objects) {

        std::unique_ptr<Object3d> newObjectData = std::make_unique<Object3d>();
        auto filePath = objectData.filePath;

        Model* model = nullptr;

        if (!filePath.directoryPath.empty()) {
            model = ModelManager::LoadModelAndGet(filePath.directoryPath + "/" + filePath.fileName);
        }

        newObjectData->Create();
        newObjectData->SetObjectType(objectType);

        if (model) {
            newObjectData->SetMeshAndMaterial(model);
        } else {
            auto* primitive = PrimitiveFactory::GetPrimitiveForName(objectData.filePath.fileName);
            newObjectData->SetMeshAndMaterial(primitive);
            //テクスチャハンドルを入れる
            newObjectData->SetTextureHandle(static_cast<TextureFactory::Handle>(objectData.materialData.textureHandle));
        }

        newObjectData->SetColor(objectData.materialData.color);
        newObjectData->SetTemperature(objectData.materialData.temperature);
        newObjectData->SetShininess(objectData.materialData.shininess);
        //環境光
        newObjectData->SetEnvironmentCoefficient(objectData.materialData.environmentCoefficient);
        //ガラス度合い
        newObjectData->SetGlassFactor(objectData.materialData.glassFactor);
        //ライトモード
        newObjectData->SetLightMode(static_cast<Object3d::LightMode>(objectData.materialData.lightMode));

        newObjectData->SetObjectName(objectData.objectName);

        //ロックされているかなどの情報を渡さない。

        newObjectData->RegisterObject();

        auto& transform = newObjectData->GetWorldTransform();

        transform.eTransform_ = objectData.transform;
        //ここで一度 Object3d の行列計算(Update)を走らせて matWorld_ を確定させる！
        newObjectData->Update();

        object3ds.push_back(std::move(newObjectData));
    }
}


void LevelEditor::LoadVector4(nlohmann::json& object, Vector4& vector)
{
    //カラーを探す
    if (object.contains("color")) {
        vector = {
            .x = (float)object["color"]["x"] ,
            .y = (float)object["color"]["y"] ,
            .z = (float)object["color"]["z"] ,
            .w = (float)object["color"]["w"]
        };
    } else {
        vector = { 1.0f,1.0f,1.0f,1.0f };
    }
}

void LevelEditor::LoadMaterialData(LevelData::MaterialData& data, nlohmann::json& object)
{
    //テクスチャハンドル
    LoadTextureHandle(object, data.textureHandle);
    //テクスチャファイルネーム
    LoadName(data.textureFilePath, object, "textureFilePath");
    //色
    LoadVector4(object, data.color);
    //温度
    LoadFloat(data.temperature, object);
    //輝度の読みこみ
    LoadFloat(data.shininess, object, "shininess");
    //環境
    LoadFloat(data.environmentCoefficient, object, "environmentCoefficient");
    //ガラス度合
    LoadFloat(data.glassFactor, object, "glassFactor");
    LoadLightMode(data.lightMode, object);

}

void LevelEditor::LoadObject(nlohmann::json& object, LevelData* levelData) {

    assert(object.contains("type"));
    //タイプ
    std::string type = object["type"].get<std::string>();


    //MESHorTree
    if (type.compare(objectTypeName_[kMesh]) == 0|| type.compare(objectTypeName_[kTree] )== 0) {

        //オブジェクト
        levelData->objects.emplace_back(LevelData::ObjectData{});
        LevelData::ObjectData& objectData = levelData->objects.back();
        //マテリアル情報
        LoadMaterialData(objectData.materialData, object);
        //名前
        LoadName(objectData.objectName, object, "name");
        //ファイルパス
        LoadMeshData(objectData.filePath, object);
        //トランスフォーム
        LoadTransform(object, objectData.transform);
        //子要素
        LoadChildren(object, levelData);
        //コライダー子要素
        LoadCollider(objectData.colliderData, object);

    } else if (type.compare(objectTypeName_[kPlayerSpawn]) == 0) {
        //ぷれいやーでーた　
        levelData->players.emplace_back(LevelData::PlayerSpawnData{});
        LevelData::PlayerSpawnData& playerData = levelData->players.back();
       //トランスフォームの読み込み
        LoadTransform(object, playerData.transform);

    } else if (type.compare(objectTypeName_[kEnemySpawn]) == 0) {
        //敵
        levelData->enemies.emplace_back(LevelData::EnemySpawnData{});
        LevelData::EnemySpawnData& enemyData = levelData->enemies.back();

        //温度
        LoadFloat(enemyData.temperature, object);
        //トランスフォームの読み込み
        LoadTransform(object, enemyData.transform);
        //子供
        LoadChildren(object, levelData);
        //ファイルパス
        LoadMeshData(enemyData.filePath, object);

    } else if (type.compare(objectTypeName_[kStageChangeTrigger]) == 0) {
        //隕∫ｴ霑ｽ蜉
        levelData->stageChangeTriggers_.emplace_back(LevelData::StageChangeTriggerData{});
        LevelData::StageChangeTriggerData& stageChangeTriggerData = levelData->stageChangeTriggers_.back();
        //マテリアル情報
        LoadMaterialData(stageChangeTriggerData.materialData, object);
        //次のステージ
        LoadName(stageChangeTriggerData.nextStageName, object, "nextStageName");
        //名前
        LoadName(stageChangeTriggerData.openFlag, object, "openFlagName");
        //オブジェクト名
        LoadName(stageChangeTriggerData.objectName, object, "name");
        //ロック
        LoadBool(stageChangeTriggerData.isLocked, object, "isLocked");
        //ファイルパス
        LoadMeshData(stageChangeTriggerData.filePath, object);
        //トランスフォーム
        LoadTransform(object, stageChangeTriggerData.transform);
        //コライダー
        LoadCollider(stageChangeTriggerData.colliderData, object);
    }

}

void LevelEditor::LoadTextureHandle(nlohmann::json& object, uint32_t& handle)
{
    if (object.contains("textureHandle")) {

        handle = object["textureHandle"];
    } else {
        handle = TextureFactory::WHITE_1X1;
    }

}

void LevelEditor::LoadLightMode( uint32_t& handle, nlohmann::json& object)
{
    if (object.contains("lightMode")) {

        handle = object["lightMode"];
    } else {
        handle = Object3d::LightMode::kLightModeHalfL;
    }
}

void LevelEditor::LoadName(std::string& fileName, nlohmann::json& object, const std::string& loadName)
{
    if (object.contains(loadName)) {
        fileName = object[loadName];
    }
}

void LevelEditor::LoadMeshData(LevelData::MeshFileData& meshData, nlohmann::json& object)
{
    //ファイルネーム
    LoadName(meshData.fileName, object);
    //ディレクトリパスを入れる
    LoadName(meshData.directoryPath, object, "directoryPath");
}

void LevelEditor::LoadFloat(float& value, nlohmann::json& object, const std::string& name)
{
    if (object.contains(name)) {
        value = object[name];
    } else {
        value = 0.0f;
    }
}

void LevelEditor::LoadBool(bool& value, nlohmann::json& object, const std::string& name)
{
    if (object.contains(name)) {
        value = object[name];
    } else {
        value = false;
    }
}

void LevelEditor::LoadTransform(nlohmann::json& object, EulerTransform& transform)
{

    if (object.contains("transform")) {
        nlohmann::json& loadTransform = object["transform"];
        //どこのデータ化によって処理を変更する
        //インデックス
        uint32_t transformIndex = 0;

        transform.translate.x = (float)loadTransform[transformsName_[transformIndex]][vector3Name_[0]];
        transform.translate.y = (float)loadTransform[transformsName_[transformIndex]][vector3Name_[1]];
        transform.translate.z = (float)loadTransform[transformsName_[transformIndex]][vector3Name_[2]];
        transformIndex++;
        //位置

        transform.rotate.x = (float)loadTransform[transformsName_[transformIndex]][vector3Name_[0]];
        transform.rotate.y = (float)loadTransform[transformsName_[transformIndex]][vector3Name_[1]];
        transform.rotate.z = (float)loadTransform[transformsName_[transformIndex]][vector3Name_[2]];
        transformIndex++;

        if (vector3Name_[2] == "y") {
            transform.rotate.x *= -1.0f;
            transform.rotate.z *= -1.0f;
        }

        //スケールを入れる
        transform.scale.x = (float)loadTransform[transformsName_[transformIndex]][vector3Name_[0]];
        transform.scale.y = (float)loadTransform[transformsName_[transformIndex]][vector3Name_[1]];
        transform.scale.z = (float)loadTransform[transformsName_[transformIndex]][vector3Name_[2]];

    }
}

void LevelEditor::LoadChildren(nlohmann::json& object, LevelData* levelData)
{
    //再帰呼び出し
    if (object.contains("children")) {

        for (nlohmann::json& child : object["children"]) {
            LoadObject(child, levelData);
        }
    }
}

void LevelEditor::LoadCollider(LevelData::ColliderData& colliderData, nlohmann::json& object)
{
    //コライダーの読み込み
    if (object.contains("collider")) {

        nlohmann::json& collider = object["collider"];
        //タグ
        if (collider.contains("tag")) {
            //タグの名前
            colliderData.tag = collider["tag"];
        }
        if (collider.contains("maskTag")) {
            //マスク処理
            colliderData.maskTag = collider["maskTag"];
        }
        if (collider.contains("type")) {
            colliderData.type = collider["type"];
        }

        colliderData.center = {
                (float)collider["center"][vector3Name_[0]],
                (float)collider["center"][vector3Name_[1]],
                (float)collider["center"][vector3Name_[2]],
        };
        colliderData.size = {
            .x = (float)collider["size"][vector3Name_[0]] ,
            .y = (float)collider["size"][vector3Name_[1]] ,
            .z = (float)collider["size"][vector3Name_[2]]
        };

    }
}


