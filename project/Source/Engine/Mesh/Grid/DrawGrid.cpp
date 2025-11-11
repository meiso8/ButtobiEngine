#include"DrawGrid.h"
#include<numbers>
#include"MakeMatrix.h"
#include"Texture.h"

std::array<LineMesh*, 102> DrawGrid::line_;
std::array < Cube*, 2> DrawGrid::cube_;

std::array<Object3d*, 104> DrawGrid::lineTransforms_;

void DrawGrid::Finalize()
{

    for (auto& line : line_) {
        delete line;
    }

    for (auto& cube : cube_) {
        delete cube;
    }


    for (auto& line : lineTransforms_) {
        delete line;
    }

}

void DrawGrid::Create()
{
    for (int i = 0; i < line_.size(); ++i) {
        line_[i] = new LineMesh();
        line_[i]->Create(Texture::WHITE_1X1);
    }

    for (int i = 0; i < 51; ++i) {
        line_[i]->SetVertexData(Vector3(-25.0f, 0.0f, static_cast<float>(i - 25)), Vector3(25.0f, 0.0f, static_cast<float>(i - 25)));
        line_[i + 51]->SetVertexData(Vector3(static_cast<float>(i - 25), 0.0f, -25.0f), Vector3(static_cast<float>(i - 25), 0.0f, 25.0f));

        if (i % 10 == 0) {
            line_[i]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            line_[i + 51]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        } else if (i == 25) {
            line_[i]->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
            line_[i + 51]->SetColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
        } else {
            line_[i]->SetColor(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
            line_[i + 51]->SetColor(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
        }
    }

    for (int i = 0; i < cube_.size(); ++i) {
        cube_[i] = new Cube();
        cube_[i]->Create(Texture::WHITE_1X1);
        cube_[i]->SetLightMode(kLightModeNone);
    }

    cube_[0]->SetMinMax({ -1.0f / 128.0f,-1.0f / 128.0f,-25.0f }, { 1.0f / 128.0f,1.0f / 128.0f,25.0f });
    cube_[1]->SetMinMax({ -25.0f,-1.0f / 128.0f,-1.0f / 128.0f }, { 25.0f,1.0f / 128.0f,1.0f / 128.0f });

    cube_[0]->SetColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
    cube_[1]->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

    for (size_t i = 0; i < lineTransforms_.size(); ++i) {
        lineTransforms_[i] = new Object3d();
        lineTransforms_[i]->Create();
        lineTransforms_[i]->Update();
        if (i >= 2) {
            lineTransforms_[i - 2]->SetMesh(line_[i - 2]);
        } else {
            lineTransforms_[i]->SetMesh(cube_[i]);
        }
    }


}

void DrawGrid::Draw(Camera& camera, bool isDraw) {

    if (!isDraw) {
        return;
    }

    for (int i = 0; i < lineTransforms_.size(); ++i) {
        lineTransforms_[i]->Draw(camera, kBlendModeNone);
    }
}