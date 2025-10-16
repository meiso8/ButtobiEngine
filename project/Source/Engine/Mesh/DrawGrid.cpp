#include"DrawGrid.h"
#include<numbers>
#include"MakeMatrix.h"
#include"Texture.h"

LineMesh DrawGrid::line_[102];
Cube DrawGrid::cube_[2];

void DrawGrid::Initialize()
{
    uint32_t textureHandle = Texture::GetHandle(Texture::WHITE_1X1);

    for (int i = 0; i < 102; ++i) {
        line_[i].Create(textureHandle);
    }

    for (int i = 0; i < 51; ++i) {
        line_[i].SetVertexData(Vector3(-25.0f, 0.0f, static_cast<float>(i - 25)), Vector3(25.0f, 0.0f, static_cast<float>(i - 25)));
        line_[i + 51].SetVertexData(Vector3(static_cast<float>(i - 25), 0.0f, -25.0f), Vector3(static_cast<float>(i - 25), 0.0f, 25.0f));

        if (i % 10 == 0) {
            line_[i].SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            line_[i + 51].SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        } else if (i == 25) {
            line_[i].SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
            line_[i + 51].SetColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
        } else {
            line_[i].SetColor(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
            line_[i + 51].SetColor(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
        }
    }

    cube_[0].Create(textureHandle);
    cube_[1].Create(textureHandle);

    cube_[0].SetMinMax({ -1.0f / 128.0f,-1.0f / 128.0f,-25.0f }, { 1.0f / 128.0f,1.0f / 128.0f,25.0f });
    cube_[1].SetMinMax({ -25.0f,-1.0f / 128.0f,-1.0f / 128.0f }, { 25.0f,1.0f / 128.0f,1.0f / 128.0f });

    cube_[0].SetColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
    cube_[1].SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}

void DrawGrid::Draw(Camera& camera) {

    Matrix4x4 identity4x4 = MakeIdentity4x4();

    line_[0].PreDraw();

    for (int i = 0; i < 102; ++i) {
        line_[i].Draw(camera, identity4x4);
    }
    cube_[0].PreDraw();

    for (int i = 0; i < 2; ++i) {
        cube_[i].Draw(camera, identity4x4);
    }

}