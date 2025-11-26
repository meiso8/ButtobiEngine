#include "Field.h"
#include"Input.h"
#include"DebugUI.h"
Field::Field()
{

    circle_ = { {0.0f,0.0f,0.0f},25.0f };
    circleMesh_ = std::make_unique<CircleMesh>();
    circleMesh_->Create(Texture::NUMBERS);
    circleMesh_->SetVertex(circle_);

    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();
    object3d_->SetMesh(circleMesh_.get());

}

void Field::Init()
{
    object3d_->Initialize();
}

void Field::Update()
{
    uvTranslate_ += InverseFPS;
    object3d_->GetUVTransform().translate.y = uvTranslate_;
    object3d_->UpdateUV();
    object3d_->Update();
    DebugUI::CheckMesh(*circleMesh_, "fieldMesh");
    DebugUI::CheckObject3d(*object3d_, "fieldObject");
}

void Field::Draw(Camera& camera)
{
    object3d_->Draw(camera);
}
