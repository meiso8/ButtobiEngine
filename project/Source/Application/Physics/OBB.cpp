#include "OBB.h"
#include"Texture.h"
#include"LineMesh.h"
#include"MakeMatrix.h"

#ifdef _DEBUG
#include "../externals/imgui/imgui.h"
#endif // _DEBUG

void SetAxis(const Vector3 &rotate, OBB &obb) {
	// 回転行列の作成
	Matrix4x4 rotationMatrix = MakeRotateXYZMatrix(rotate);

	// 各軸の設定
	obb.axis[0] = { rotationMatrix.m[0][0], rotationMatrix.m[0][1], rotationMatrix.m[0][2] };
	obb.axis[1] = { rotationMatrix.m[1][0], rotationMatrix.m[1][1], rotationMatrix.m[1][2] };
	obb.axis[2] = { rotationMatrix.m[2][0], rotationMatrix.m[2][1], rotationMatrix.m[2][2] };
}

#ifdef _DEBUG
void EditOBB(const std::string &label, Vector3 &rotate, OBB &obb) {
	if (ImGui::TreeNode(label.c_str())) {
		ImGui::Text("Center");
		ImGui::DragFloat3("Center", &obb.center.x, 0.1f);
		ImGui::Separator();
		ImGui::Text("Rotate");
		ImGui::SliderAngle("Rotate X", &rotate.x, -180.0f, 180.0f);
		ImGui::SliderAngle("Rotate Y", &rotate.y, -180.0f, 180.0f);
		ImGui::SliderAngle("Rotate Z", &rotate.z, -180.0f, 180.0f);
		SetAxis(rotate, obb);
		ImGui::Separator();
		ImGui::Text("Axis");
		ImGui::DragFloat3("Axis X", &obb.axis[0].x, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat3("Axis Y", &obb.axis[1].x, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat3("Axis Z", &obb.axis[2].x, 0.01f, -1.0f, 1.0f);
		ImGui::Separator();
		ImGui::Text("Half Sizes");
		ImGui::DragFloat3("Half Sizes", &obb.halfSizes.x, 0.1f, 0.0f);
		ImGui::TreePop();
	}
}
#endif // _DEBUG

OBBRenderer::OBBRenderer() = default;
OBBRenderer::~OBBRenderer() = default;

void OBBRenderer::Initialize() {
	uint32_t textureHandle = Texture::GetHandle(Texture::WHITE_1X1);
	for (auto &line : lines_) {
		line = std::make_unique<LineMesh>();
		line->Create(textureHandle);
	}
	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	for (auto &line : lines_) {

		line->SetColor(color);
	}
}

void OBBRenderer::Update(const OBB &obb) {
	// OBBの8頂点を計算
	Vector3 vertices[8];
	Vector3 halfSizes = obb.halfSizes;
	Vector3 axisX = obb.axis[0] * halfSizes.x;
	Vector3 axisY = obb.axis[1] * halfSizes.y;
	Vector3 axisZ = obb.axis[2] * halfSizes.z;
	vertices[0] = obb.center + axisX + axisY + axisZ;	// 0: +X, +Y, +Z
	vertices[1] = obb.center - axisX + axisY + axisZ;	// 1: -X, +Y, +Z
	vertices[2] = obb.center + axisX - axisY + axisZ;	// 2: +X, -Y, +Z
	vertices[3] = obb.center - axisX - axisY + axisZ;	// 3: -X, -Y, +Z
	vertices[4] = obb.center + axisX + axisY - axisZ;	// 4: +X, +Y, -Z
	vertices[5] = obb.center - axisX + axisY - axisZ;	// 5: -X, +Y, -Z
	vertices[6] = obb.center + axisX - axisY - axisZ;	// 6: +X, -Y, -Z
	vertices[7] = obb.center - axisX - axisY - axisZ;	// 7: -X, -Y, -Z

	// 線分メッシュの頂点を設定
	lines_[0]->SetVertexData(vertices[0], vertices[1]);		// 上面前辺
	lines_[1]->SetVertexData(vertices[1], vertices[3]);		// 上面左辺
	lines_[2]->SetVertexData(vertices[3], vertices[2]);		// 上面奥辺
	lines_[3]->SetVertexData(vertices[2], vertices[0]);		// 上面右辺
	lines_[4]->SetVertexData(vertices[4], vertices[5]);		// 底面前辺
	lines_[5]->SetVertexData(vertices[5], vertices[7]);		// 底面左辺
	lines_[6]->SetVertexData(vertices[7], vertices[6]);		// 底面奥辺
	lines_[7]->SetVertexData(vertices[6], vertices[4]);		// 底面右辺
	lines_[8]->SetVertexData(vertices[0], vertices[4]);		// 前面左辺
	lines_[9]->SetVertexData(vertices[1], vertices[5]);		// 前面右辺
	lines_[10]->SetVertexData(vertices[3], vertices[7]);	// 奥面左辺
	lines_[11]->SetVertexData(vertices[2], vertices[6]);	// 奥面右辺
}

void OBBRenderer::Draw(Camera &camera) {
	// 線分メッシュの描画
	lines_[0]->PreDraw();
	for (auto &line : lines_) {
		line->Draw(camera, MakeIdentity4x4());
	}
}