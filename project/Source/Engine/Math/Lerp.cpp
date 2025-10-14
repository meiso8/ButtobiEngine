#include "Lerp.h"
#include<cmath>
#include"Texture.h"
#include"MakeMatrix.h"
#include"LineMesh.h"
#include"Camera/Camera.h"

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {

    Vector3 normalizeV1 = Normalize(v1);
    Vector3 normalizeV2 = Normalize(v2);
    float dot = Dot(normalizeV1, normalizeV2);
    //dot = std::min(dot, 1.0f);
    float phi = std::acosf(dot);

    return std::sinf((1.0f - t) * phi) / std::sinf(phi) * normalizeV1 + std::sinf(t * phi) / std::sinf(phi) * normalizeV2;
};


void BezierMesh::Create(const Vector3& p0, const Vector3& p1, const Vector3& p2, const uint32_t divisionNum)
{

    lineMesh_.resize(divisionNum);

    for (uint32_t i = 0; i < divisionNum; i++) {
        float t0 = i / static_cast<float>(divisionNum);
        float t1 = (i + 1) / static_cast<float>(divisionNum);

        //t0とt1と制御点を使ってベジェ曲線上の点を求める
        Vector3 bezier0 = Bezier(p0, p1, p2, t0);
        Vector3 bezier1 = Bezier(p0, p1, p2, t1);
        lineMesh_[i].Create(Texture::handle_[Texture::WHITE_1X1]);
        lineMesh_[i].SetVertexData(bezier0, bezier1);
    }


}

void BezierMesh::Draw(Camera& camera, const Vector4& color)
{
    Matrix4x4 worldMatrix = MakeIdentity4x4();

    lineMesh_[0].PreDraw();

    for (uint32_t i = 0; i < lineMesh_.size();++i) {
  
        lineMesh_[i].SetColor(color);
        lineMesh_[i].Draw(camera, worldMatrix);
    }

}
