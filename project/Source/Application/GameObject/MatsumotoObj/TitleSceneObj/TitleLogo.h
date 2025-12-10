#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include <vector>
#include <memory>

class Model;
class Camera;
enum LightMode;

class TitleLogo
{
public:
	TitleLogo();
	~TitleLogo();
	void Initialize();
	void Update();
	void Draw(Camera& camera);

	Object3d body_;
	std::vector<std::unique_ptr<Object3d>> logos_;
private:
};