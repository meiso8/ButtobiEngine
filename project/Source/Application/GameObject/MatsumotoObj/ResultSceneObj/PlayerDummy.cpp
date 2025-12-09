#include "PlayerDummy.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>
#include"Collision.h"
#include "Input.h"
#include"CollisionConfig.h"
#include"JsonFile.h"
#include "MatsumotoObj/MY_Utility.h"
PlayerDummy::PlayerDummy()
{
    body_.Create();
    headObject_.Create();
    rightArmObject_.Create();
    leftArmObject_.Create();
    rightLegObject_.Create();
    leftLegObject_.Create();

    body_.SetMesh(ModelManager::GetModel(ModelManager::PLAYER_BODY));
    headObject_.SetMesh(ModelManager::GetModel(ModelManager::PLAYER_HEAD));
    rightArmObject_.SetMesh(ModelManager::GetModel(ModelManager::PLAYER_ARM_R));
    leftArmObject_.SetMesh(ModelManager::GetModel(ModelManager::PLAYER_ARM_L));
    rightLegObject_.SetMesh(ModelManager::GetModel(ModelManager::PLAYER_LEG_R));
    leftLegObject_.SetMesh(ModelManager::GetModel(ModelManager::PLAYER_LEG_L));

    headObject_.worldTransform_.Parent(body_.worldTransform_);
    rightArmObject_.worldTransform_.Parent(body_.worldTransform_);
    leftArmObject_.worldTransform_.Parent(body_.worldTransform_);
    rightLegObject_.worldTransform_.Parent(body_.worldTransform_);
    leftLegObject_.worldTransform_.Parent(body_.worldTransform_);

    headObject_.worldTransform_.translate_ = { 0.0f,0.5f,0.0f };
    rightArmObject_.worldTransform_.translate_ = { 0.5f,0.0f,0.0f };
    leftArmObject_.worldTransform_.translate_ = { -0.5f,0.0f,0.0f };
    rightLegObject_.worldTransform_.translate_ = { 0.3f,-0.5f,-0.3f };
    leftLegObject_.worldTransform_.translate_ = { -0.3f,-0.5f,-0.3f };
}

PlayerDummy::~PlayerDummy()
{
}

void PlayerDummy::Initialize()
{
	body_.worldTransform_.translate_ = { 0.0f,-0.5f,0.0f };
	body_.worldTransform_.rotate_ = { 0.0f,0.0f,0.0f };
    rightArmObject_.worldTransform_.translate_ = {0.5f,1.0f,0.0f};
    rightLegObject_.worldTransform_.translate_ = { 0.3f,-0.3f,0.0f };
    leftLegObject_.worldTransform_.translate_ = { -0.3f,-0.3f,0.0f };
}

void PlayerDummy::Update()
{
	body_.worldTransform_.rotate_.y += 0.02f;

	body_.Update();
	headObject_.Update();
	rightArmObject_.Update();
	leftArmObject_.Update();
	rightLegObject_.Update();
	leftLegObject_.Update();
}

void PlayerDummy::Draw(Camera& camera)
{
	body_.Draw(camera);
	headObject_.Draw(camera);
	rightArmObject_.Draw(camera);
	leftArmObject_.Draw(camera);
	rightLegObject_.Draw(camera);
	leftLegObject_.Draw(camera);
}
