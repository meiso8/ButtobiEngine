#include "Lerp.h"
#include"Normalize.h"
#include"Dot.h"
#include<cmath>

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {

    Vector3 normalizeV1 = Normalize(v1);
    Vector3 normalizeV2 = Normalize(v2);
    float dot = Dot(normalizeV1, normalizeV2);
    //dot = std::min(dot, 1.0f);
    float phi = std::acosf(dot);

    return std::sinf((1.0f - t) * phi) / std::sinf(phi) * normalizeV1 + std::sinf(t * phi) / std::sinf(phi) * normalizeV2;
};

