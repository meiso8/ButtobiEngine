#include "ExprostionParticle.h"
#include "ModelManager.h"
#include "MyEngine.h"
#include "Camera.h"
#include "Object3d.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

#include"CubeMesh.h"
#include "MatsumotoObj/MY_Utility.h"
#include"CircleMesh.h" 

ExprostionParticle::ExprostionParticle() {
    body_.Create();
    // It's good practice to initialize variables.
    isActive_ = false;
    size_ = 1.0f;
    lifeTimer_ = 0.0f;
    lifeDuration_ = 1.0f; // Default lifetime of 1 second.
	circleMesh_ = std::make_unique<CircleMesh>();
	circleMesh_->Create(Texture::WHITE_1X1);
}

ExprostionParticle::~ExprostionParticle() {
    // No explicit cleanup needed for models from ModelManager
    // or the unique_ptr for circleMesh_.
}

void ExprostionParticle::Initialize() {
    // Initialize the Object3d instance.
    body_.Initialize();

    // Load the sphere model and assign it to our object.
    // I am assuming a model named "SPHERE" exists in the ModelManager.
    // If this name is incorrect, you may need to change it.
    //Model* sphereModel = ModelManager::GetModel(ModelManager::BOX);
    //if (sphereModel) {
    //    
    //}
    body_.SetMesh(circleMesh_.get());

    // Set an initial scale.
    body_.worldTransform_.scale_ = { size_, size_, size_ };
}

void ExprostionParticle::Update() {
    // Only update if the particle is active.
    if (!isActive_) {
        return;
    }

    // Countdown the lifetime.
    lifeTimer_ -= 1.0f / 60.0f; // Assuming 60 FPS.
	body_.worldTransform_.scale_ = MY_Utility::SimpleEaseIn(
		body_.worldTransform_.scale_,
		{ 0.0f,0.0f,0.0f },
		0.1f);


    // If the lifetime has run out, deactivate the particle.
    if (lifeTimer_ <= 0.0f) {
        isActive_ = false;
        lifeTimer_ = 0.0f;
    }

    // Optional: Add some animation, like scaling up or fading out.
    // For now, it just appears and disappears.
    
    // Update the world transform of the object.
    body_.Update();
}

void ExprostionParticle::Draw(Camera& camera, const LightMode& lightType) {
    // Only draw if the particle is active.
    if (!isActive_) {
        return;
    }

    body_.SetLightMode(lightType);
    body_.Draw(camera);
}

void ExprostionParticle::SetPosition(const Vector3& position) {
    // Set the position of the particle.
    body_.worldTransform_.translate_ = position;

    // Activate the particle.
    isActive_ = true;

    // Reset its lifetime.
    lifeTimer_ = lifeDuration_;

    body_.worldTransform_.scale_ = { size_, size_, size_ };
}
