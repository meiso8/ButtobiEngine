#include "MummyStage.h"

void MummyStage::Initialize() {
    mummy_ = std::make_unique<Mummy>();
    mummy_->Initialize();
}

void MummyStage::Update() {
    mummy_->Update();
}

void MummyStage::Draw(Camera& camera) {
    mummy_->Draw(camera);
}
