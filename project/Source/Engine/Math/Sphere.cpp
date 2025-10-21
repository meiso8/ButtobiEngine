#include "Sphere.h"
#include "Texture.h"
#include "LineMesh.h"
#include "MakeMatrix.h"

#ifdef _DEBUG
#include "../externals/imgui/imgui.h"
#endif // _DEBUG

#ifdef _DEBUG
void EditSphere(const std::string &label, Sphere &sphere) {
	if (ImGui::TreeNode(label.c_str())) {
		ImGui::DragFloat3("Center", &sphere.center.x, 0.1f);
		ImGui::DragFloat("Radius", &sphere.radius, 0.1f, 0.1f, 100.0f);
		ImGui::TreePop();
	}
}
#endif // _DEBUG

SphereRenderer::SphereRenderer() = default;
SphereRenderer::~SphereRenderer() = default;

void SphereRenderer::Initialize() {
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

void SphereRenderer::Update(const Sphere &sphere) {
	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex; // 現在の緯度

		// 経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery; // 現在の経度
			float nextLon = lon + kLonEvery;
			float nextLat = lat + kLatEvery;

			// a,b,cのworld座標系を求める
			Vector3 a, b, c;
			a = { std::cos(lat) * std::cos(lon), std::sin(lat), std::cos(lat) * std::sin(lon) };
			b = { std::cos(nextLat) * std::cos(lon), std::sin(nextLat), std::cos(nextLat) * std::sin(lon) };
			c = { std::cos(lat) * std::cos(nextLon), std::sin(lat), std::cos(lat) * std::sin(nextLon) };
			a = sphere.center + sphere.radius * a;
			b = sphere.center + sphere.radius * b;
			c = sphere.center + sphere.radius * c;

			lines_[latIndex * kSubdivision + lonIndex]->SetVertexData(a, b);
			lines_[(latIndex + kSubdivision) * kSubdivision + lonIndex]->SetVertexData(a, c);
		}
	}
}

void SphereRenderer::Draw(Camera& camera) {
	lines_[0]->PreDraw();
	for (auto& line : lines_) {
		line->Draw(camera, MakeIdentity4x4());
	}
}