#include "Field.h"
#include"ModelManager.h"
#include"Model.h"

Field::Field()
{
    //仮
    model_ = ModelManager::GetModel(ModelManager::WORLD);

    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();
    //仮
    object3d_->SetMesh(model_);
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
