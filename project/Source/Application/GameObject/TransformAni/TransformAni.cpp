#include "TransformAni.h"
#include"Window.h"

void TransformAni::RotateY(WorldTransform& worldTransform, const float& rotateSpeed)
{
    worldTransform.rotate_.x = 1.0f;
    worldTransform.rotate_.y += InverseFPS * std::numbers::pi_v<float>*rotateSpeed;

}
