#include "Field.h"
#include"Input.h"

Field::Field()
{
    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();

    circleMesh_ = std::make_unique<CircleMesh>();
    circleMesh_->Create(Texture::NUMBERS);
    circle_ = { {0.0f,0.0f,0.0f},25.0f };
    circleMesh_->SetVertex(circle_);

    object3d_->SetMesh(circleMesh_.get());

}

void Field::Init()
{
    object3d_->Initialize();
}

void Field::Update()
{
    uvTranslate_ += InverseFPS;
    circleMesh_->SetUVTranslate({ 0.0f,uvTranslate_ ,0.0f });
    object3d_->Update();
    circleMesh_->UpdateUV();
}

void Field::Draw(Camera& camera)
{
    object3d_->Draw(camera);
}
