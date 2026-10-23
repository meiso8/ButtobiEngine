#include "SkinningModel.h"
#include"Model.h"
#include"DirectXCommon.h"
#include"Log.h"
#include"CbvSrvUavDescriptorHeap.h"
SkinningModel::SkinningModel()
{
    skeleton_ = std::make_unique<Skeleton>();
    skinCluster_ = std::make_unique<SkinCluster>();
}

SkinningModel::~SkinningModel()
{

    Skin::ReleaseSkinCluster(*skinCluster_);

    skinningCSResource_.inputVertexResource_.Reset();
  
    skinningCSResource_.outputVertexResource_.Reset();

    skinningCSResource_.skinningInformationResource_.Reset();

}

void SkinningModel::SetDescriptorHeap(CbvSrvUavDescriptorHeap* cbvSrvUavDescriptorHeap)
{
    cbvSrvUavDescriptorHeap_ = cbvSrvUavDescriptorHeap;
}

void SkinningModel::SetModel(const Model* model)
{
    //モデルのメッシュ
    modelData_ = model->GetModelData();
    //スケルトンやクラスターを作成する
    *skeleton_ = Bone::CreateSkeleton(modelData_->rootNode);
    *skinCluster_ = Skin::CreateSkinCluster(*skeleton_, *modelData_);
    
    CreateSkinningInformation();
    CreateInputVertexResource();
    CreateOutVertexResource();

}

void SkinningModel::SetRootSignatureAndGraphicsPipeline(
    ID3D12GraphicsCommandList* commandList,
    const BlendMode& blendMode,
    const CullMode& cullMode,
    const MaskMode maskMode,
    const bool usePSOKey,
    const RootSignature::TYPE rootSignatureType,
    const DxcCompiler::VS_TYPE vsType,
    const DxcCompiler::PS_TYPE psType
)
{
    (void)maskMode;
    (void)usePSOKey;
    (void)rootSignatureType;
    (void)vsType;
    (void)psType;
    //InputLayout kInputLayoutTypeSkinning;

    commandList->SetGraphicsRootSignature(PSO::GetRootSignature()->GetRootSignature(RootSignature::NORMAL));
    commandList->SetPipelineState(PSO::GetGraphicsPipelineState(blendMode, cullMode).Get());//PSOを設定

}

Matrix4x4* SkinningModel::GetJointMatrix(const std::string jointName)
{
    return  Bone::GetJointMatrix(jointName, *skeleton_);
}

void SkinningModel::CreateInputVertexResource()
{

    UINT vertexBufferSize = sizeof(VertexData) *UINT( modelData_->vertices.size());
    skinningCSResource_.inputVertexResource_.CreateBufferResource(L"inputVertexResource", vertexBufferSize);

    //頂点リソースに頂点をマッピング
    skinningCSResource_.inputVertexResource_.Map();

    std::memcpy(skinningCSResource_.inputVertexResource_.data, modelData_->vertices.data(), vertexBufferSize); // vectorの中身を一気にコピー！
    skinningCSResource_.inputVertexResource_.UnMap();
    skinningCSResource_.inputVertexResource_.Allocate(cbvSrvUavDescriptorHeap_);
    skinningCSResource_.inputVertexResource_.CreateSRVforStructuredBuffer(cbvSrvUavDescriptorHeap_, UINT(modelData_->vertices.size()));

    LogFile::Log("Create InputVertexresource");
}

void SkinningModel::CreateOutVertexResource()
{

    UINT vertexBufferSize = sizeof(VertexData) *UINT( modelData_->vertices.size());
    //UAVの作成をするよ
    skinningCSResource_.outputVertexResource_.CreateBufferResourceForUAV(L"outputVertexResource",vertexBufferSize);
    skinningCSResource_.outputVertexResource_.AllocateUAV(cbvSrvUavDescriptorHeap_);
    skinningCSResource_.outputVertexResource_.CreateUAV(cbvSrvUavDescriptorHeap_,UINT(modelData_->vertices.size()));

    // 頂点バッファビューの作成 上書き
    vertexBufferView_.BufferLocation = skinningCSResource_.outputVertexResource_.GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = vertexBufferSize;
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

    LogFile::Log("Create  OutputVertexresource");
}
void SkinningModel::CreateSkinningInformation()
{
    skinningCSResource_.skinningInformationResource_.CreateBufferResource(L"inputVertexResource");
    //書き込むためのアドレスを取得
    skinningCSResource_.skinningInformationResource_.Map();
    LogFile::Log("Create SkinningInformation");
    skinningCSResource_.skinningInformationResource_.data->numVertices = static_cast<uint32_t>(modelData_->vertices.size());
}