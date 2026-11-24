#pragma once
#include"Collider.h"
#include <memory>
#include"Object3d.h"

class TicketMac: public Collider
{

public:
    TicketMac();
    ~TicketMac();
    void Initialize();
    void Update();
    void Draw();
    void OnCollision(Collider* collider) override;
    const Vector3 GetWorldPosition();
    void SetOutline(const bool flag);
private:
    std::unique_ptr<Object3d> object_ = nullptr;
};

