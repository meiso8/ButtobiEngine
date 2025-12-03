#include "ExprostionParticleEmitter.h"
#include "Camera.h" // Included for the Draw method signature.
#include <memory>   // For std::make_unique

ExprostionParticleEmitter::ExprostionParticleEmitter() {
    // Reserve space in the vector to avoid reallocations.
    particles_.reserve(kNumParticles);
    // Create the particle instances using unique_ptr.
    for (int i = 0; i < kNumParticles; ++i) {
        particles_.emplace_back(std::make_unique<ExprostionParticle>());
    }
}

ExprostionParticleEmitter::~ExprostionParticleEmitter() {
    // unique_ptr will handle its own memory.
}

void ExprostionParticleEmitter::Initialize() {
    // Initialize each particle in the pool.
    for (const auto& particle : particles_) {
        particle->Initialize();
    }
}

void ExprostionParticleEmitter::Update() {
    // Update each particle in the pool.
    // The particle's own update logic will handle active/inactive state.
    for (const auto& particle : particles_) {
        if (particle->isActive_) {
            particle->Update();
        }
    }
}

void ExprostionParticleEmitter::Draw(Camera& camera, const LightMode& lightType) {
    // Draw each particle in the pool.
    // The particle's own draw logic will handle active/inactive state.
    for (const auto& particle : particles_) {
        if (particle->isActive_) {
            particle->Draw(camera, lightType);
        }
    }
}

void ExprostionParticleEmitter::Spawn(const Vector3& position) {
    // Find an inactive particle and activate it.
    for (const auto& particle : particles_) {
        if (!particle->isActive_) {
            particle->SetPosition(position);
            // Particle is now active, so we can stop looking.
            return;
        }
    }
    // If we reach here, no inactive particles were found.
    // Consider logging this event if it's important.
}