#include "TreeGenerator.h"
#include"Random.h"
#include"Object3d.h"
#include"ImGui.h"
#include"ModelManager.h"
#include"Primitive.h"
#include"Model.h"
#include"DebugUI.h"
#include"LevelEditor/LevelEditor.h"
#include"JsonFile.h"
#include"SceneManager.h"
#include"Collision.h"

namespace {
    std::string jsonFileName_ = "TreeGenerator1";
}
TreeGenerator::TreeGenerator()
{


}

TreeGenerator::~TreeGenerator()
{
}

void TreeGenerator::Initialize()
{

    LevelEditor::GetInstance()->Load(jsonFileName_, true);
    LevelEditor::GetInstance()->CreateObject3d(trees_);

    LoadAABB();

}
void TreeGenerator::LoadAABB()
{
    // クリアして再読み込み
    excludeAreas_.clear();

    // 保存されている JSON データを取得
    nlohmann::json& json = JsonFile::GetJsonFiles(jsonFileName_);

    // "colliders" が存在し、かつ配列構造になっているか確認
    if (json.contains("colliders") && json["colliders"].is_array()) {
        for (const auto& item : json["colliders"]) {
            AABB aabb;

            if (item.contains("min") && item.contains("max")) {
                // Vector3 としてそのまま読み込む
                aabb.min = JsonFile::JsonToVector3(item["min"]);
                aabb.max = JsonFile::JsonToVector3(item["max"]);

                excludeAreas_.push_back(aabb);
            }
        }
    }
}

void TreeGenerator::Update()
{

    for (auto& tree : trees_) {
        tree->Update();
    }
}

void TreeGenerator::Draw()
{
    for (auto& tree : trees_) {
        tree->Draw();
    }
}

void TreeGenerator::Debug()
{

    ImGui::Begin("TreeGenerator");

    ImGui::SliderInt("generateNum", &generateNum_, 0, 1000);
    ImGui::SliderFloat("minDistance", &minDistance_, -100000, 100000);
    ImGui::SliderFloat("rangeMax", &rangeMax_, rangeMin_, 100000);
    ImGui::SliderFloat("rangeMin", &rangeMin_, -100000, rangeMax_);
  
    if (ImGui::Button("Generate")) {
        PlaceLockersRandomly();
    }

    if (ImGui::Button("Save")) {
        Save();
    }

    if (ImGui::TreeNode("Tree")) {

        for (auto& tree : trees_) {

            if (ImGui::TreeNode(tree->GetObjectName().c_str())) {


                DebugUI::CheckWorldTransform(tree->GetWorldTransform(), "WorldTransform");

                auto* primitive = tree->GetPrimitive();

                if (primitive) {

                    //プリミティブならテクスチャをセットできる
                    int textureIndex = tree->GetTextureHandle();
                    if (ImGui::SliderInt("texture", &textureIndex, 0, TextureFactory::TEXTURES)) {
                        //テクスチャのセット
                        tree->SetTextureHandle(static_cast<TextureFactory::Handle>(textureIndex));
                    };

                    std::string currentModelName = "unknow";
                    if (auto model = dynamic_cast<Model*>(primitive)) {

                        for (const auto& [name, managerModel] : ModelManager::GetModels()) {
                            if (model == managerModel.get()) {
                                currentModelName = name.string().c_str();
                            }
                        }
                    }

                    if (ImGui::BeginCombo("Set Model", currentModelName.c_str())) {
                        // マップ内のすべてのシーンをループして選択肢を作る
                        for (const auto& [name, scene] : ModelManager::GetModels()) {
                            // 選択肢を表示（クリックされたら true を返す）
                            if (ImGui::Selectable(name.string().c_str(), true)) {
                                // クリックされたらシーン切り替え関数を呼ぶ
                                tree->SetMeshAndMaterial(ModelManager::GetModel(name));
                                break;
                            }
                        }

                        ImGui::EndCombo();
                    }
                }

                ImGui::TreePop();
            }

        }

        ImGui::TreePop();
    }
   
    if (ImGui::TreeNode("AABB")) {

        for (int32_t i = 0; i < excludeAreas_.size();++i) {
            ImGui::PushID(i);
            DebugUI::CheckAABB(excludeAreas_[i]);
            ImGui::PopID();
        }

        if (ImGui::Button("Add AABB")) {
            excludeAreas_.push_back(AABB{ .min = {-1.0f,-1.0f,-1.0f},.max = {1.0f,1.0f,1.0f} });
        }

        if (ImGui::Button("Save")) {
            SaveAABB();
        }

        ImGui::TreePop();
    }

    ImGui::End();
}

void TreeGenerator::Save()
{
    nlohmann::json& json = JsonFile::GetJsonFiles(jsonFileName_);
    std::string sceneName = SceneManager::GetCurrentSceneName() + "Scene";
    //シーン名
    json["name"] = sceneName;

    // 一度配列をクリアする
    json["objects"] = nlohmann::json::array();

    for (auto& object : trees_) {

        // オブジェクト名の取得
        std::string name = object->GetObjectName() + std::to_string(object->GetObjectID());

        std::string meshName = "empty";

        auto* primitive = object->GetPrimitive();
        if (primitive) {
            meshName = primitive->GetMeshName();
        }

        nlohmann::json objectJson = {
            {"file_name", meshName},
            {"transform", JsonFile::EulerTransformToJson(object->GetTransform())},
            {"disabled", object->GetDisabled()},
            {"lightMode",object->GetLightMode()},
            {"temperature",object->GetTemperature()},
            {"color",JsonFile::Vector4ToJson(object->GetColor())},
            {"shininess",object->GetShininess()},
            {"environmentCoefficient",object->GetEnvironmentCoefficient()},
             {"glassFactor",object->GetGlassFactor()},
            {"name", name},
            {"type", object->GetObjectType() },
            {"nextStageName",object->GetNextStageName()},

        };

        //輝度を追加

        //モデルだった場合ディレクトリパスの要素を追加
        if (auto model = dynamic_cast<Model*>(primitive)) {
            objectJson["directoryPath"] = model->GetModelData()->directoryPath_;
        } else {
            //テクスチャハンドルを設定する
            objectJson["textureHandle"] = object->GetTextureHandle();
        }

        // 4. 配列に要素を追加
        json["objects"].push_back(objectJson);
    }

    // ファイル保存
    JsonFile::SaveJson(jsonFileName_);
    JsonFile::MarkModified(jsonFileName_);

}

void TreeGenerator::SaveAABB()
{
    nlohmann::json& json = JsonFile::GetJsonFiles(jsonFileName_);
    std::string sceneName = SceneManager::GetCurrentSceneName() + "Scene";
    //シーン名
    json["name"] = sceneName;

    // 一度配列をクリアする
    json["colliders"] = nlohmann::json::array();

    int i = 0;

    for (auto& aabb : excludeAreas_) {

        nlohmann::json objectJson = {
            {"min", JsonFile::Vector3ToJson(aabb.min)},
            {"max", JsonFile::Vector3ToJson(aabb.max)},
        };

        // 4. 配列に要素を追加
        json["colliders"].push_back(objectJson);
        
        i++;
    }

    // ファイル保存
    JsonFile::SaveJson(jsonFileName_);
    JsonFile::MarkModified(jsonFileName_);
}

void TreeGenerator::PlaceLockersRandomly() {


    trees_.clear();

    std::vector<Vector2> placedPositions;
    Model* model = ModelManager::LoadModelAndGet("Resource/Models/Tree/Tree.obj");

    Random random;
    random.SetMinMax(rangeMin_, rangeMax_);

    Random rotateRanDom;
    rotateRanDom.SetMinMax(0.0f, std::numbers::pi_v<float>);


    for (int32_t i = 0; i < generateNum_; ++i) {

        Vector2 pos;
        bool positionFound = false;

        // 無限ループ防止のため最大試行回数を設定 (例: 1000回)
        const int maxTries = 1000;
        int tries = 0;

        while (tries < maxTries) {

            pos.x = random.Get();
            pos.y = random.Get(); // Z座標として使う 

            // 1. 建物の領域内でないか
            // 2. 他の木と重なっていないか
            if (!IsInExcludeArea(pos) && !IsOverlapping(pos, placedPositions)) {
                positionFound = true;
                break; // 配置成功
            }

            tries++;
        }

        // 配置可能な位置が見つかった場合のみ作成
        if (positionFound) {
            placedPositions.push_back(pos);

            std::unique_ptr<Object3d> tree = std::make_unique<Object3d>();
            tree->Create();
            tree->SetRotate({ 0.0f, rotateRanDom.Get() ,0.0f });
            tree->SetMeshAndMaterial(model);
            tree->SetObjectName("tree" + std::to_string(i));
            tree->SetObjectType("MESH");
            tree->GetWorldTransform().eTransform_.translate = { pos.x, 0.0f, pos.y };
            trees_.push_back(std::move(tree));

        }
    }
}


bool TreeGenerator::IsOverlapping(const Vector2& pos, const std::vector<Vector2>& placedPositions) {
    for (const auto& p : placedPositions) {
        if (Distance(p, pos) < minDistance_ * minDistance_) {
            return true;
        }
    }
    return false;
}

bool TreeGenerator::IsInExcludeArea(const Vector2& pos) const
{
 
    for (const auto& area : excludeAreas_) {
        float testY = (area.min.y + area.max.y) * 0.5f;
        Vector3 position = { pos.x,testY,pos.y };

        if (IsCollision(area, position)) {
            return true; // 除外エリア内
        }
    }
    return false;
}
