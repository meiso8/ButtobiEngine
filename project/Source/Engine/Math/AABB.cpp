#include"AABB.h"
#include"Texture.h"
#include"LineMesh.h"
#include"MakeMatrix.h"

#ifdef _DEBUG
#include "../externals/imgui/imgui.h"
#endif // _DEBUG

#ifdef _DEBUG
void EditAABB(const std::string &label, AABB &aabb) {
	if (ImGui::TreeNode(label.c_str())) {
		ImGui::DragFloat3("min", &aabb.min.x, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat3("max", &aabb.max.x, 0.1f, -100.0f, 100.0f);
		ImGui::TreePop();
	}
}
#endif // _DEBUG

AABBRenderer::AABBRenderer() = default;
AABBRenderer::~AABBRenderer() = default;

void AABBRenderer::Initialize() {
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

void AABBRenderer::Update(const AABB &aabb) {
	Vector3 vertices[8] = {
		{aabb.min.x, aabb.min.y, aabb.min.z},
		{aabb.max.x, aabb.min.y, aabb.min.z},
		{aabb.max.x, aabb.max.y, aabb.min.z},
		{aabb.min.x, aabb.max.y, aabb.min.z},
		{aabb.min.x, aabb.min.y, aabb.max.z},
		{aabb.max.x, aabb.min.y, aabb.max.z},
		{aabb.max.x, aabb.max.y, aabb.max.z},
		{aabb.min.x, aabb.max.y, aabb.max.z}
	};

	// 12本の線を引く
	lines_[0]->SetVertexData(vertices[0], vertices[1]);
	lines_[1]->SetVertexData(vertices[1], vertices[2]);
	lines_[2]->SetVertexData(vertices[2], vertices[3]);
	lines_[3]->SetVertexData(vertices[3], vertices[0]);
	lines_[4]->SetVertexData(vertices[4], vertices[5]);
	lines_[5]->SetVertexData(vertices[5], vertices[6]);
	lines_[6]->SetVertexData(vertices[6], vertices[7]);
	lines_[7]->SetVertexData(vertices[7], vertices[4]);
	lines_[8]->SetVertexData(vertices[0], vertices[4]);
	lines_[9]->SetVertexData(vertices[1], vertices[5]);
	lines_[10]->SetVertexData(vertices[2], vertices[6]);
	lines_[11]->SetVertexData(vertices[3], vertices[7]);
}

void AABBRenderer::Draw(Camera &camera) {
	lines_[0]->PreDraw();
	for (auto &line : lines_) {
		line->Draw(camera, MakeIdentity4x4());
	}
}