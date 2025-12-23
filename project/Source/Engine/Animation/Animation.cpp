#include "Animation.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
//#include<assimp/postprocess.h>
#include<cassert>


Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename)
{
    std::string filePath = directoryPath + "/" + filename;
    return  LoadAnimationFileForFilePath(filePath);
}

Animation LoadAnimationFileForFilePath(const std::string& filePath)
{
    Animation animation;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
    assert(scene->mNumAnimations != 0);//アニメーションがない
    aiAnimation* animationAssimp = scene->mAnimations[0];//最初のアニメーションだけ採用。自分へ複数対応は後程検討します。
    //mDuration : mTicksPerSecondで指定された周波数における長さ、mTicksPerSecond : 周波数
    animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);//時間の単位を秒に変換　

    {
        //NodeAnimatioinの解析
         //assimpでは個々のNodeのAnimationをchannelと読んでいるのでchannelを回してnodeAnimetionの情報を取ってくる
        for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
            aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
            NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

            //translate
            for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
                aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
                KeyframeVector3 keyframe;
                keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);//ここも秒に変換
                keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };//右手->左手
                nodeAnimation.translate.keyframes.push_back(keyframe);
            }

            //rotate
            for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
                aiQuatKey keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
                KeyframeQuaternion keyframe;
                keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);//ここも秒に変換
                keyframe.value = { keyAssimp.mValue.x,-keyAssimp.mValue.y,-keyAssimp.mValue.z ,keyAssimp.mValue.w };//右手->左手
                nodeAnimation.rotate.keyframes.push_back(keyframe);
            }

            //scale
            for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
                aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
                KeyframeVector3 keyframe;
                keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);//ここも秒に変換
                keyframe.value = { keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };//右手->左手
                nodeAnimation.scale.keyframes.push_back(keyframe);
            }

        }

    }

    return animation;
}

// Vector3用のLerp
Vector3 Interpolate(const Vector3& a, const Vector3& b, float t) {
    return Lerp(a, b, t);
}

// Quaternion用のSlerp
Quaternion Interpolate(const Quaternion& a, const Quaternion& b, float t) {
    return Slerp(a, b, t);
}

//Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, const float& time)
//{
//    assert(!keyframes.empty());//キーがない
//    if (keyframes.size() == 1 || time <= keyframes[0].time) {
//        return keyframes[0].value;
//    }
//
//
//    for (size_t index = 0; index < keyframes.size() - 1; ++index) {
//        size_t nextIndex = index + 1;
//        //indexとnextIndexの2つのkeyframeを取得して範囲内二次国があるかを判別
//        if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
//            // 範囲内に補間する
//            float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
//            return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
//        }
//    }
//
//    //最後まで行っちゃっった場合は最後の値を返すよ
//    return (*keyframes.rbegin()).value;
//}
