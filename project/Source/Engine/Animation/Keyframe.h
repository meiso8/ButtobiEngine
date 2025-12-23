#pragma once
#include"Vector3.h"
#include"Quaternion/Quaternion.h"
//struct KeyframeVector3 {
//    Vector3 value;
//    float time;
//};
//
//struct KeyframeQuaternion {
//    Quaternion value;//キーフレームの値
//    float time;//キーフレームの時刻(単位は秒)
//};

template <typename tValue>

struct Keyframe {
    float time;
    tValue value;
};

using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;