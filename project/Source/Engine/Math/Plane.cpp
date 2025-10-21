#include "Plane.h"
#include "Texture.h"
#include "LineMesh.h"
#include "Perpendicular.h"
#include "MakeMatrix.h"
#include <cassert>

#ifdef _DEBUG
#include "../externals/imgui/imgui.h"

void EditPlane(const std::string &label, Plane &plane) {
	if (ImGui::TreeNode(label.c_str())) {
		ImGui::DragFloat3("normal", &plane.normal.x, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat("distance", &plane.distance, 0.1f, -100.0f, 100.0f);
		ImGui::TreePop();
	}
}
#endif // _DEBUG

PlaneRenderer::PlaneRenderer()
{
	uint32_t textureHandle = Texture::GetHandle(Texture::WHITE_1X1);
	for (auto& line : lines_) {
		line = std::make_unique<LineMesh>();
		line->Create(textureHandle);
	}

}

PlaneRenderer::~PlaneRenderer() = default;

void PlaneRenderer::Initialize() {

	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	for (auto &line : lines_) {
		line->SetColor(color);
	}
}

void PlaneRenderer::Update(const Plane &plane) {
	Vector3 center = plane.normal * plane.distance;	// 平面の中心
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));	// 平面の法線に垂直なベクトル
	perpendiculars[1] = -perpendiculars[0];							// perpendiculars[0]の逆ベクトル
	perpendiculars[2] = Cross(perpendiculars[0], plane.normal);	// 法線とperpendiculars[0]の外積
	perpendiculars[3] = -perpendiculars[2];							// perpendiculars[2]の逆ベクトル
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = perpendiculars[index] * 16.0f;
		points[index] = center + extend;
	}

	lines_[0]->SetVertexData(points[0], points[2]);
	lines_[1]->SetVertexData(points[0], points[3]);
	lines_[2]->SetVertexData(points[1], points[2]);
	lines_[3]->SetVertexData(points[1], points[3]);
}

void PlaneRenderer::Draw(Camera &camera) {
	lines_[0]->PreDraw();
	for (auto &line : lines_) {
		line->Draw(camera, MakeIdentity4x4());
	}
}