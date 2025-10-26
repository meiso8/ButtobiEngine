#pragma once
#include "WorldTransform.h"
#include <memory>
#include <array>

struct Plane;
struct OBB;

class Camera;
class Model;
class PlaneRenderer;
class OBBRenderer;

class Stage {
public:
    static inline constexpr uint32_t kMaxPlane = 2;
    static inline constexpr uint32_t kMaxOBB = 4;

private:
    std::unique_ptr<Model> model_ = nullptr;
    WorldTransform worldTransform_;

    // 平面
    std::array<std::unique_ptr<Plane>, kMaxPlane> planes_;
    std::array<std::unique_ptr<PlaneRenderer>, kMaxPlane> planeRenderers_;

    // OBB
    std::array<Vector3, kMaxOBB> obbRotates_;
    std::array<std::unique_ptr<OBB>, kMaxOBB> obbs_;
    std::array<std::unique_ptr<OBBRenderer>, kMaxOBB> obbRenderers_;

public:
    Stage();
    ~Stage();

    void Initialize();
    void Update();
    void Draw(Camera& camera);

    Plane GetPlane(uint32_t index);
    OBB GetOBB(uint32_t index);
};