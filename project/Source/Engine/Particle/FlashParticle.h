#include"Particle.h"
class Camera;

class FlashParticle :public ParticleManager {
public:
    ~FlashParticle()override;
    void Update(Camera& camera)override;
};