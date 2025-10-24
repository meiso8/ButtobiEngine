#include"FlashParticle.h"
#include"Camera/Camera.h"

FlashParticle::~FlashParticle()
{
    Finalize();
}

void FlashParticle::Update(Camera& camera)
{

    UpdateBillBordMatrix(camera);

    numInstance_ = 0;

    for (std::list <Particle>::iterator particleIterator = particles.begin(); particleIterator != particles.end();) {

        if (numInstance_ < kNumMaxInstance) {
            if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
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

            Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, camera.GetViewProjectionMatrix());


            instancingData[numInstance_].WVP = worldViewProjectionMatrix;
            instancingData[numInstance_].World = worldMatrix;
            instancingData[numInstance_].color = (*particleIterator).color;
            float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime)*2.0f;
            instancingData[numInstance_].color.w = alpha;




            ++numInstance_;
        }
        ++particleIterator;
    }


}

