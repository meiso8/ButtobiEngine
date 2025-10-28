#include"FlashParticle.h"
#include"Camera/Camera.h"
#include"Camera/SpriteCamera.h"

FlashParticle::~FlashParticle()
{
    Finalize();
}

void FlashParticle::Update(Camera& camera)
{
    if (useSpriteCamera_) {
        useBillboard_ = false;
        //ビルボードをしない
    } else {
        UpdateBillBordMatrix(camera);
    }

    numInstance_ = 0;

    for (std::list <Particle>::iterator particleIterator = particles.begin(); particleIterator != particles.end();) {

        if (numInstance_ < kNumMaxInstance) {
            if (0.5f <= (*particleIterator).currentTime) {
                particleIterator = particles.erase(particleIterator);
                continue;
            }

            IsCollisionFieldArea(*particleIterator);

            (*particleIterator).transform.translate += (*particleIterator).velocity * kDeltaTime;

            (*particleIterator).currentTime += kDeltaTime;

            if (useBillboard_) {
                UpdateWorldMatrixForBillBord(*particleIterator);
            } else {
                UpdateWorldMatrix(*particleIterator);
            }
            if (useSpriteCamera_) {
                worldViewProjectionMatrix = Multiply(worldMatrix, SpriteCamera::GetViewProjectionMatrix());
            } else {
                worldViewProjectionMatrix = Multiply(worldMatrix, camera.GetViewProjectionMatrix());
            }

            instancingData[numInstance_].WVP = worldViewProjectionMatrix;
            instancingData[numInstance_].World = worldMatrix;
            instancingData[numInstance_].color = (*particleIterator).color;
        /*    float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime) * 2.0f;
            instancingData[numInstance_].color.w = alpha;*/

            ++numInstance_;
        }
        ++particleIterator;
    }


}

