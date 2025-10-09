#pragma once

#include"ImGuiClass.h"
#include"Vector3.h"
#include"Vector4.h"
#include"WorldTransform.h"
#include<list>

enum BlendMode;
class Input;
class Sprite;
class SphereMesh;
class Camera;
class Model;

class DebugUI
{
public:

    /// @brief モデル
    /// @param model 
    static void CheckModel(Model& model);
    /// @brief スプライト
    /// @param sprite 
    static void CheckSprite(Sprite& sprite);
    /// @brief 球
    /// @param sphere 
    static void CheckSphere(SphereMesh& sphere);
    /// @brief ワールドトランスフォーム
    /// @param worldTransform 
    static void CheckWorldTransform(WorldTransform& worldTransform);
    /// @brief ワールドマトリックス
    /// @param scale 
    /// @param rotate 
    /// @param translate 
    /// @param label 
    static void CheckWorldMatrix(Vector3& scale, Vector3& rotate, Vector3& translate, const char* label);
    /// @brief 入力
    /// @param input 
    static void CheckInput(Input& input);
    /// @brief カメラ
    /// @param camera 
    static void CheckCamera(Camera& camera);
    /// @brief 光源
    /// @param directionalLights 
    /// @param lightType 
    static void CheckDirectionalLight(uint32_t& lightType);
    /// @brief 色
    /// @param color 
    static void CheckColor(Vector4& color);

    static void CheckBlendMode(uint32_t& blendMode);
    static void CheckInt(int& value);
    static void CheckFPS();


};

