#include "Field.h"
#include"ModelManager.h"
#include"Model.h"

Field::Field()
{
    //仮
    model_ = ModelManager::GetModel(ModelManager::WORLD);

    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();

    circleMesh_ = std::make_unique<CircleMesh>();
    circleMesh_->Create(Texture::UV_CHECKER);
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
    object3d_->Update();
}

void Field::Draw(Camera& camera)
{
    object3d_->Draw(camera);
}
