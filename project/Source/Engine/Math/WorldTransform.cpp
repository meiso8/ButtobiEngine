#include "WorldTransform.h"  
#include"MakeMatrix.h"

void WorldTransform::Initialize() {

    scale_ = { 1.0f,1.0f,1.0f };
    rotate_ = { 0.0f,0.0f,0.0f };
    translate_ = { 0.0f,0.0f,0.0f };
    matWorld_ = MakeIdentity4x4();
}


void WorldTransformUpdate(WorldTransform& worldTransform) {

    // 親となるワールド変換へのポインタ

    worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotate_, worldTransform.translate_);

    // 親があれば親のワールド行列をかける
    if (worldTransform.parent_) {
        worldTransform.matWorld_ = Multiply(worldTransform.matWorld_, worldTransform.parent_->matWorld_);
    }
}





