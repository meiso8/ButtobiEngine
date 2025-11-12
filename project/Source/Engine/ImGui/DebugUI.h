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
#include<memory>
#include<unordered_map>

enum BlendMode;
class Input;
class Sprite;
class SphereMesh;
class Camera;
class Model;
class ParticleManager;
class ParticleEmitter;
class Object3d;
struct Material;
struct PointLight;
class MeshCommon;
struct CharacterState;


class DebugUI
{
public:

    static void CheckCharacterState(CharacterState& state,const char* label);

    /// @brief メッシュ
    /// @param mesh 
    static void CheckMesh(MeshCommon& mesh,const char* label);
    /// @brief モデル
    /// @param model
    /// @param label 
    static void CheckModel(Model& model, const char* label);
    /// @brief スプライト
    /// @param sprite 
    static void CheckSprite(Sprite& sprite, const char* label);
    static void CheckBalloonData(Balloon& balloon);
    static void CheckWaveData(Wave& wave, const char* label);
    static void CheckPointLightData(PointLight& pointLight, const char* label);

    static void CheckObject3d(Object3d& object3d, const char* label);

    static void CheckParticle(ParticleEmitter& particleEmitter);
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
    static void CheckDirectionalLight();
    static void CheckMaterial(Material& material, const char* label);
    static void CheckLightMode(int32_t& lightMode, const char* label);

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

