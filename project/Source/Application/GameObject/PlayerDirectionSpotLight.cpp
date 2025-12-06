#include "PlayerDirectionSpotLight.h"
#include"Lights/SpotLightManager.h"

PlayerDirectionSpotLight::PlayerDirectionSpotLight()
{
    SpotLightManager::GetData()->intensity = 1.0f;
    SpotLightManager::GetData()->cosAngle = 0.99f;
   
    pos_.translate_.z = -1.0f;
}
void PlayerDirectionSpotLight::Update()
{
    SpotLightManager::GetData()->direction = *dirPtr_;
   
    WorldTransformUpdate(pos_);

    SpotLightManager::GetData()->position = pos_.GetWorldPosition();


}
