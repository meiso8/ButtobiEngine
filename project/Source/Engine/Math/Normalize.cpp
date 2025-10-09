#include"Normalize.h"
#include"Length.h"

Vector3 Normalize(const Vector3& v) {
    float length = Length(v);
    if (length != 0.0f) {
        return { v.x / length,v.y / length,v.z / length };
    } else {
        return { 0.0f, 0.0f, 0.0f };
    }
  
};

Vector2 Normalize(const Vector2& v) {
    float length = Length(v);
    if (length != 0.0f) {
        return { v.x / length,v.y / length };
    } else {
        return { 0.0f, 0.0f};
    }
};

//template<typename Type> Type Normalize(Type& v) {
//
//    float length = Length(v)
//
//};