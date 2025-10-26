#include "WorldTransform.h"  
#include"MakeMatrix.h"

void WorldTransform::Initialize() {

    scale_ = { 1.0f,1.0f,1.0f };
    rotate_ = { 0.0f,0.0f,0.0f };
    translate_ = { 0.0f,0.0f,0.0f };
    matWorld_ = MakeIdentity4x4();
}

Vector3 WorldTransform::GetWorldPosition() const
{
    return { matWorld_.m[3][0], matWorld_.m[3][1], matWorld_.m[3][2] };
}

void WorldTransform2D::Initialize()
{
    scale_ = { 1.0f,1.0f };
    rotate_ = { 0.0f };
    translate_ = { 0.0f,0.0f };
    matWorld_ = MakeIdentity3x3();
}

Vector2 WorldTransform2D::GetWorldPosition() const
{
    return { matWorld_.m[2][0], matWorld_.m[2][1] };
}

void WorldTransformUpdate(WorldTransform& worldTransform)
{
    // 親となるワールド変換へのポインタ
    worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotate_, worldTransform.translate_);

    // 親があれば親のワールド行列をかける
    if (worldTransform.parent_) {
        worldTransform.matWorld_ = Multiply(worldTransform.matWorld_, worldTransform.parent_->matWorld_);
    }
}

void WorldTransformUpdate(WorldTransform2D& worldTransform2D)
{
    // 親となるワールド変換へのポインタ
    worldTransform2D.matWorld_ = MakeAffineMatrix(worldTransform2D.scale_, worldTransform2D.rotate_, worldTransform2D.translate_);

    // 親があれば親のワールド行列をかける
    if (worldTransform2D.parent_) {
        worldTransform2D.matWorld_ = Multiply(worldTransform2D.matWorld_, worldTransform2D.parent_->matWorld_);
    }
}
