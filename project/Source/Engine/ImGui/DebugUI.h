#pragma once

#include"ImGuiClass.h"
#include"Vector3.h"
#include"Vector4.h"
#include"WorldTransform.h"
#include<list>
#include"Transform.h"
#include"Balloon.h"
#include"Wave.h"
#include <functional>

enum BlendMode;
class Input;
class Sprite;
class SphereMesh;
class Camera;
class Model;
class ParticleMesh;

class DebugUI
{
public:

    /// @brief モデル
    /// @param model 
    static void CheckModel(Model& model,const char* label);
    /// @brief スプライト
    /// @param sprite 
    static void CheckSprite(Sprite& sprite, const char* label);
    static void CheckBalloonData(Balloon& balloon);
    static void CheckWaveData(Wave& wave, const char* label);
    static void CheckParticle(ParticleMesh& particle,const char* label);
    /// @brief ワールドトランスフォーム
    /// @param worldTransform 
    static void CheckWorldTransform(WorldTransform& worldTransform,const char* label);
    /// @brief ワールドマトリックス
    /// @param scale 
    /// @param rotate 
    /// @param translate 
    /// @param label 
    static void CheckTransform(Transform& transform, const char* label);

    static void CheckTransforms(Vector3& scale, Vector3& rotate, Vector3& translate, const char* label);
    //行列の要素を見る関数
    static void ShowMatrix4x4(const Matrix4x4& matrix, const char* label = "Matrix4x4");
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
    static void CheckColor(Vector4& color, const char* label);

    static void CheckBlendMode(uint32_t& blendMode);
    static void CheckInt(int& value, const char* label);
    static void CheckFPS();
    static void CheckFlag(bool& flag, const char* label);
    static void SwitchFlag(bool& flag, const char* label);
    static void Button(const char* label, std::function<void()> onSwitch);
};

