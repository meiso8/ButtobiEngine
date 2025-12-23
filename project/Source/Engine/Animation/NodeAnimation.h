#pragma once
#include<vector>
#include"Keyframe.h"

template <typename tValue>
struct AnimatioinCurve {
    std::vector<Keyframe<tValue>> keyframes;
};

struct NodeAnimation {
    AnimatioinCurve<Vector3> translate;
    AnimatioinCurve<Quaternion> rotate;
    AnimatioinCurve<Vector3> scale;
};


