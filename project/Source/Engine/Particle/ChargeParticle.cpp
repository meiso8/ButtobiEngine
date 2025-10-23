#include "ChargeParticle.h"
#include"Random.h"
#include<numbers>
#include"Camera/Camera.h"

std::list<SphericalCoordinate> EmitCoordinate(const Emitter& emitter)
{
    std::list<SphericalCoordinate>sphericalCoordinates;
    for (uint32_t count = 0; count < emitter.cont; ++count) {
        sphericalCoordinates.push_back(ChargeParticle::MakeNewSphericalCoordinate());
    }
    return sphericalCoordinates;

}


ChargeParticle::~ChargeParticle()
{

    Finalize();
}

SphericalCoordinate ChargeParticle::MakeNewSphericalCoordinate()
{
    SphericalCoordinate sphericalCoordinate;
    Random::SetMinMax(0.0f, 6.28f);
    sphericalCoordinate.azimuthal = 0.0f;
    sphericalCoordinate.polar = Random::Get();
    sphericalCoordinate.radius = 3.0f;
    return sphericalCoordinate;
}

void ChargeParticle::EmitParticle(const Vector4& color)
{
    for (uint32_t count = 0; count < emitter_.cont; ++count) {
        particles.push_back(MakeNewParticle(emitter_.transform.translate, color));
        sphericalCoordinates.push_back(MakeNewSphericalCoordinate());
    }

}


void ChargeParticle::Update(Camera& camera)
{

    auto particleIterator = particles.begin();
    auto coordIterator = sphericalCoordinates.begin();

    while (particleIterator != particles.end() && coordIterator != sphericalCoordinates.end()) {
        coordIterator->polar += std::numbers::pi_v<float> *kDeltaTime * 4.0f;

        if (coordIterator->radius > 0.0f) {
            coordIterator->radius -= kDeltaTime * 4.0f;
        } else {
            coordIterator->radius = 5.0f;
        }

        particleIterator->transform.translate = emitter_.transform.translate + TransformCoordinate(*coordIterator);

        ++particleIterator;
        ++coordIterator;
    }

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

            float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);

            instancingData[numInstance_].color.w = alpha;

            ++numInstance_;
        }
        ++particleIterator;
    }


}