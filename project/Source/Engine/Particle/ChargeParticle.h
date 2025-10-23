#pragma once

#include"Particle.h"

#include"SphericalCoordinate.h"

std::list<SphericalCoordinate> EmitCoordinate(const Emitter& emitter);

class ChargeParticle :public ParticleManager {
public:


    std::list<SphericalCoordinate>sphericalCoordinates;

    ~ChargeParticle()override;

    static SphericalCoordinate MakeNewSphericalCoordinate();
    void EmitParticle(const Vector4& color)override;
    void Update(Camera& camera)override;
};
