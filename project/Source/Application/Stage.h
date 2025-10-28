#pragma once
#include "WorldTransform.h"
#include <memory>
#include <array>
#include"Vector4.h"
struct Plane;
struct OBB;
class OBBCube;
class Camera;

class PlaneRenderer;
class OBBRenderer;

class Stage {
public:
    static inline constexpr uint32_t kMaxPlane = 2;
    static inline constexpr uint32_t kMaxOBB = 4;

private:

    WorldTransform worldTransform_;

    // 平面
    std::array<std::unique_ptr<Plane>, kMaxPlane> planes_;
    std::array<std::unique_ptr<PlaneRenderer>, kMaxPlane> planeRenderers_;

    // OBB
    std::array<Vector3, kMaxOBB> obbRotates_;
    std::array<std::unique_ptr<OBB>, kMaxOBB> obbs_;
    std::array<std::unique_ptr<OBBRenderer>, kMaxOBB> obbRenderers_;

    std::array < std::unique_ptr<OBBCube>, kMaxOBB> obbCubes_;	// 立方体メッシュ
    Vector4 color_;
    bool isAlpha_ = false;
public:
    Stage();
    ~Stage();

    void Initialize();
    void Update();
    void Draw(Camera& camera);

    Plane GetPlane(uint32_t index);
    OBB GetOBB(uint32_t index);

    void IsSetAlphaFalse();
};