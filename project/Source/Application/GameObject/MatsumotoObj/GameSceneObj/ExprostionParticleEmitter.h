#pragma once
#include "ExprostionParticle.h"
#include <vector>
#include <memory> // For std::unique_ptr
#include "MyEngine.h" // For Vector3

// Forward declarations
class Camera;
enum LightMode;

class ExprostionParticleEmitter {
public:
    ExprostionParticleEmitter();
    ~ExprostionParticleEmitter();

    // Initializes the emitter and all particles in the pool.
    void Initialize();

    // Updates all active particles.
    void Update();

    // Draws all active particles.
    void Draw(Camera& camera, const LightMode& lightType);

    // Spawns a particle at the specified position.
    void Spawn(const Vector3& position);

private:
    // Pool of particles managed by this emitter.
    // We use unique_ptr because ExprostionParticle is non-copyable.
    std::vector<std::unique_ptr<ExprostionParticle>> particles_;

    // The maximum number of particles in the pool.
    static const int kNumParticles = 100;
};
