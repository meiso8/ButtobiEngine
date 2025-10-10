#pragma once
#include"MeshCommon.h"

class Cube :public MeshCommon
{
public:
    void SetMinMax(const Vector3& min, const Vector3& max);
private:
    void CreateVertex()override;
    void CreateIndexResource()override;
};

