#pragma once
#include"Vector3.h"
#include"MakeMatrix.h"
#include"LineMesh.h"
#include"Lerp.h"
#include"Line.h"



Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
    Vector3 result;
    result = Add(segment.origin, Project(Subtract(point, segment.origin), segment.diff));
    return result;
};


//正射影ベクトル　射影 proj b a
Vector3 Project(const Vector3& v1, const Vector3& v2) {
    Vector3 result;

    Vector3 normalV2 = Normalize(v2);
    //長さ* 方向
    result = Multiply(Dot(v1, normalV2), normalV2);
    return result;

};