#include "DebugUI.h"
#include"MyEngine.h"
#include"Input.h"
#include"Sprite.h"
#include"Model.h"
#include"Particle/Particle.h"
#include"Particle/ParticleEmitter.h"
#include"Object3d.h"

#include"SphereMesh.h"
#include"DirectionalLight.h"
#include"PSO.h"
#include"Camera.h"

#include<numbers>
#include<algorithm>


void DebugUI::CheckInt(int& value, const char* label) {

    if (ImGui::TreeNode(label)) {
        ImGui::SliderInt(label, &value, -100, 100);
        ImGui::TreePop();
    }
}

void DebugUI::CheckCamera(Camera& camera) {

    if (ImGui::TreeNode("Camera")) {

        CheckTransforms(camera.scale_, camera.rotate_, camera.translate_, "worldMatrix");
        ShowMatrix4x4(camera.worldMat_);
        if (ImGui::Button("InitTransform")) {
            camera.InitializeTransform();
        }
        if (ImGui::TreeNode("ShericalCoordinate")) {
            ImGui::SliderFloat("polar", &camera.sphericalCoordinate_.polar, -10.0f, 10.0f);
            ImGui::SliderFloat("azimuthal", &camera.sphericalCoordinate_.azimuthal, -10.0f, 10.0f);
            ImGui::SliderFloat("radius", &camera.sphericalCoordinate_.radius, -100.0f, 100.0f);
            ImGui::TreePop();
        }

        ImGui::SliderFloat2("ofsset", &camera.offset_.x, -1000.0f, 1000.0f);
        ImGui::SliderFloat("nearZ", &camera.nearZ_, 0.0f, 1000.0f);
        ImGui::SliderFloat("farZ", &camera.farZ_, 0.0f, 1000.0f);

        ImGui::Text("Type : %s", (camera.projectionType_ == Camera::PERSPECTIVE) ? "PERSPECTIVE" : "PARALLEL");

        if (ImGui::Button("ChangeType")) {
            camera.projectionType_ = (camera.projectionType_ == Camera::PERSPECTIVE) ? Camera::PARALLEL : Camera::PERSPECTIVE;
        }

        ImGui::TreePop();

    }

}

void DebugUI::CheckModel(Model& model, const char* label) {

    ImGui::Begin("Model");

    if (ImGui::TreeNode(label)) {

        CheckWaveData(model.GetWaveData(0), "wave0");
        CheckWaveData(model.GetWaveData(1), "wave1");

        CheckBalloonData(model.GetBalloonData());

        CheckTransform(model.GetUVTransform(), "UVTransform");
        CheckColor(model.GetColor(), "modelColor");

        ImGui::TreePop();
    }

    ImGui::End();
}

void DebugUI::CheckInput(Input& input) {

    ImGui::Begin("Input");

    float x = 100;
    float y = 100;
    input.GetJoyStickPos(&x, &y, Input::BUTTON_LEFT);
    ImGui::Text("normLX:%f %f", x, y);
    float dX = 100;
    float dY = 100;
    input.GetJoyStickDPadButton(&dX, &dY);

    ImGui::Text("Dpad:%f %f", dX, dY);

    ImGui::Text("joyStateLX: %ld", input.GetJoyState().lX);//x軸位置
    ImGui::Text("joyStateLY: %ld", input.GetJoyState().lY);
    ImGui::Text("joyStateLZ: %ld", input.GetJoyState().lZ);
    ImGui::Text("joyStateRX: %ld", input.GetJoyState().lRx);//右スティック
    ImGui::Text("joyStateRY: %ld", input.GetJoyState().lRy);

    if (input.GetJoyState().rgdwPOV[0] != -1) {//十字キー　角度を表す
        ImGui::Text("POV[%d]: %lu", 0, input.GetJoyState().rgdwPOV[0]);
    } else {
        ImGui::Text("POV[%d]: Centered", 0);
    }

    for (int i = 0; i < 12; ++i) {
        ImGui::Text("Button[%d]: %s", i,
            (input.GetJoyState().rgbButtons[i] & 0x80) ? "Pressed" : "Released");
    }

    ImGui::End();

}

void DebugUI::CheckSprite(Sprite& sprite, const char* label) {
    ImGui::Begin("Sprite");

    if (ImGui::TreeNode(label)) {

        if (ImGui::TreeNode("transform2D")) {
            ImGui::SliderFloat2("pos", &sprite.GetPosition().x, -1280.0f, 1280.0f);
            ImGui::SliderFloat("rotation", &sprite.GetRotate(), 0.0f, std::numbers::pi_v<float>*2.0f);
            ImGui::SliderFloat2("size", &sprite.GetSize().x, -1280.0f, 1280.0f);
            ImGui::TreePop();
        }

        CheckTransforms(sprite.GetUVScale(), sprite.GetUVRotate(), sprite.GetUVTranslate(), "uvTransform");

        if (ImGui::TreeNode("anchorPointTextureSize")) {

            ImGui::SliderFloat2("anchorPoint", &sprite.GetAnchorPoint().x, 0.0f, 1.0f);
            ImGui::Checkbox("isFlipX", &sprite.GetIsFlipX());
            ImGui::Checkbox("isFlipY", &sprite.GetIsFlipY());
            ImGui::SliderFloat2("textureLeftTop", &sprite.GetTextureLeftTop().x, 0.0f, 1280.0f);
            ImGui::SliderFloat2("textureSize", &sprite.GetTextureSize().x, 0.0f, 1280.0f);
            ImGui::TreePop();
        }

        CheckColor(sprite.GetColor(), "color");

        ImGui::TreePop();
    }



    ImGui::End();
}

void DebugUI::ShowMatrix4x4(const Matrix4x4& matrix, const char* label) {
    if (ImGui::BeginTable(label, 4, ImGuiTableFlags_Borders)) {
        for (uint32_t row = 0; row < 4; ++row) {
            ImGui::TableNextRow();
            for (uint32_t col = 0; col < 4; ++col) {
                ImGui::TableSetColumnIndex(col);
                ImGui::Text("%.3f", matrix.m[row][col]);
            }
        }
        ImGui::EndTable();
    }
}


void DebugUI::CheckBalloonData(Balloon& balloon)
{
    if (ImGui::TreeNode("Balloon")) {
        ImGui::DragFloat("expansionData", &balloon.expansion, 0.03f, 0.0f, 10.0f);
        ImGui::DragFloat("sphere", &balloon.sphere, 0.03f, 0.0f, 1.0f);
        ImGui::DragFloat("cube", &balloon.cube, 0.03f, 0.0f, 1.0f);
        ImGui::Checkbox("isSphere", &balloon.isSphere);
        ImGui::TreePop();
    }
}
void DebugUI::CheckWaveData(Wave& wave, const char* label)
{
    if (ImGui::TreeNode(label)) {

        ImGui::DragFloat("time", &wave.time, 0.03f);
        ImGui::DragFloat("amplitude", &wave.amplitude, 0.03f);
        ImGui::DragFloat3("direction", &wave.direction.x, 0.03f, 0.0f, 1.0f);
        ImGui::SliderFloat("frequency", &wave.frequency, 1.0f, 10.0f);
        Vector3 waveDirection = wave.direction;
        wave.direction = Normalize(waveDirection);
        ImGui::TreePop();
    }
}
void DebugUI::CheckObject3d(Object3d& object3d, const char* label)
{
    ImGui::Begin("Object3d");
    CheckWorldTransform(object3d.worldTransform_, label);
    ImGui::End();
}
void DebugUI::CheckParticle(ParticleManager& particle,ParticleEmitter& particleEmitter)
{

    ImGui::Begin("Particle");

    ImGui::Checkbox("useBillboard", &particle.useBillboard_);

    Emitter& emitter = particleEmitter.emitter_;
    int count = emitter.cont;
    ImGui::SliderInt("createNum", &count, 0, particle.kNumMaxInstance);
    emitter.cont = count;

    CheckTransform(emitter.transform, "EmitterTransform");

    ImGui::SliderFloat("frequency", &emitter.frequency, 0.1f, 10.0f);
    ImGui::Text("frequencyTime : %f", emitter.frequencyTime);
    
    for (const auto& [name, group] : particle.GetParticleGroups()) {

        if (ImGui::Button(name.c_str())) {
            particle.EmitParticle(name, emitter.transform.translate, emitter.cont);
        }

        for (std::list<Particle>::iterator itr = group->particles.begin(); itr != group->particles.end(); ++itr) {
            CheckTransform((*itr).transform, name.c_str());
        }
    }


    ImGui::End();
}

void DebugUI::CheckTransforms(Vector3& scale, Vector3& rotate, Vector3& translate, const char* label) {

    if (ImGui::TreeNode(label)) {
        ImGui::SliderFloat3("scale", &scale.x, 0.0f, 10.0f);
        ImGui::SliderFloat3("rotation", &rotate.x, 0.0f, std::numbers::pi_v<float>*2.0f);
        ImGui::SliderFloat3("translation", &translate.x, -1000.0f, 1000.0f);
        ImGui::TreePop();
    }
};
void DebugUI::CheckColor(Vector4& color, const char* label) {

    if (ImGui::TreeNode(label)) {
        ImGui::ColorEdit4("color", (float*)&color);
        ImGui::TreePop();
    }
}

void DebugUI::CheckTransform(Transform& transform, const char* label)
{
    CheckTransforms(transform.scale, transform.rotate, transform.translate, label);
}

void DebugUI::CheckWorldTransform(WorldTransform& worldTransform, const char* label) {

    CheckTransforms(worldTransform.scale_, worldTransform.rotate_, worldTransform.translate_, label);

};

void DebugUI::CheckDirectionalLight(uint32_t& lightType) {

    if (ImGui::TreeNode("DirectionalLight")) {
        DirectionalLight* directionalLight = MyEngine::GetDirectionalLightData();
        Vector3 direction = directionalLight->direction;

        ImGui::ColorEdit4("color", &directionalLight->color.x);
        ImGui::SliderFloat3("direction", &direction.x, -1.0f, 1.0f);//後で正規化する
        directionalLight->direction = Normalize(direction);
        ImGui::DragFloat("intensity", &directionalLight->intensity);

        const char* lights[] = { "NONE", "LambertianReflectance", "HalfLambert" };

        static int light_current = 0;

        ImGui::Combo("LightMode", &light_current, lights, IM_ARRAYSIZE(lights));
        lightType = light_current % 3;

        ImGui::TreePop();
    }

};

void DebugUI::CheckBlendMode(uint32_t& blendMode) {

    if (ImGui::TreeNode("BlendMode")) {

        const char* blendModes[] = {
            "kBlendModeNone",
            "kBlendModeNormal",
            "kBlendModeAdd",
            "kBlendModeSubtract",
            "kBlendModeMultiply",
            "kBlendModeScreen"
        };

        static int blendMode_current = 1;

        ImGui::Combo("blendMode", &blendMode_current, blendModes, IM_ARRAYSIZE(blendModes));
        blendMode = blendMode_current % 6;
        ImGui::TreePop();
    }

};

void DebugUI::CheckFPS() {
    ImGui::Text("FPS : %f", ImGui::GetIO().Framerate);
}

void DebugUI::CheckFlag(bool& flag, const char* label)
{
    std::string labels = std::string(label) + " : " + (flag ? "true" : "false");
    ImGui::Text("%s", labels.c_str());
}

void DebugUI::SwitchFlag(bool& flag, const char* label)
{
    CheckFlag(flag, label);

    if (ImGui::Button(label)) {
        // スペースキーを押すとデバッグカメラに切り替える
        flag = flag ? false : true;
    }


}
void DebugUI::Button(const char* label, std::function<void()> onSwitch)
{
    if (ImGui::Button(label)) {
        if (onSwitch) {
            onSwitch(); // ボタンが押されたら関数オブジェクトを実行！
        }
    }
}
