#pragma once
struct Matrix4x4;
struct Vector3;
//四元数
struct Quaternion {
    float x;
    float y;
    float z;
    float w;

    bool operator==(const Quaternion& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
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
//任意軸回移転を表すQuaternionの生成
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, const float& angle);
//ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
//Quaternionから回転行列を求める
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

//球面線形補間
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, const float& t);

Quaternion Subtract(const Quaternion& q0, const Quaternion& q1);
Quaternion Add(const Quaternion& q0, const Quaternion& q1);
float Dot(const Quaternion& q0, const Quaternion& q1);

Quaternion operator+(const Quaternion& q0, const Quaternion& q1);

Quaternion operator-(const Quaternion& q);
Quaternion operator*(const Quaternion& v, float s);
Quaternion operator*(float s, const Quaternion& v);
