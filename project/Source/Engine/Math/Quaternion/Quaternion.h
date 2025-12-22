#pragma once

//四元数
struct Quaternion {
    float x;
    float y;
    float z;
    float w;
};

//xyzw は実数　ijkは虚数単位
//wが実部（スカラー部）、xi,yj,zkを虚部（ベクトル部）と呼ぶ
//q  = w + xi + yj + zk ;

/// @brief Quaternionの積
/// @param lhs 
/// @param rhs 
/// @return 
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
/// @brief 単位Quaternion
/// @return 
Quaternion IdentityQuaternion();
/// @brief 共役Quaternion
/// @param quaternion 
/// @return 
Quaternion Conjugate(const Quaternion& quaternion);
/// @brief Quaternionのnormを返す
/// @param quaternion 
/// @return 
float Norm(const Quaternion& quaternion);
/// @brief 正規化したQuaternion
/// @param quaternion 
/// @return 単位Quaternionとなる
Quaternion Normalize(const Quaternion& quaternion);
/// @brief 逆Quaternionを返す
/// @param quaternion 
/// @return 
Quaternion Inverse(const Quaternion& quaternion);

