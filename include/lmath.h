#ifndef __IMATH_H__
#define __IMATH_H__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string>

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define clamp(a,b,c) min(max((a),(b)),(c))

#define PI 3.14159265358979323846264338327950288f
#define EPSILON 1e-6f

namespace Lurdr {

typedef class Vector2 vec2;
typedef class Vector3 vec3;
typedef class Vector4 vec4;
typedef class Quaternion quat;

/**
 * Vector2
 */
class Vector2 {
public:
    float x, y;
    Vector2(): x(0), y(0) {}
    Vector2(float x, float y): x(x), y(y) {}
    Vector2(const Vector2 & vec): x(vec.x), y(vec.y) {}

    /* Static Members */
    static Vector2 UNIT_X;
    static Vector2 UNIT_Y;
    static Vector2 ZERO;

    /* Operator Overload Functions */
    Vector2 operator+ (const Vector2 & other) const;
    Vector2 operator- () const;
    Vector2 operator- (const Vector2 & other) const;
    Vector2 operator* (const float & multiplicand) const;
    Vector2 operator/ (const float divisor) const;
    bool operator== (const Vector2 & other) const;
    bool operator!= (const Vector2 & other) const;

    /* Member Functions */
    float dot(const Vector2 & other) const;
    Vector2 cross(const Vector2 & other) const;
    float length() const;
    Vector2 normalized() const;
    void normalize();
    void print();

    /* Static Functions */
    static Vector2 lerp(const Vector2 & from, const Vector2 & to, float alpha);
};

/**
 * Vector3
 */
class Vector3 {
public:
    float x, y, z;
    Vector3(): x(0), y(0), z(0) {}
    Vector3(float x, float y, float z): x(x), y(y), z(z) {}
    Vector3(const Vector3 & vec): x(vec.x), y(vec.y), z(vec.z) {}

    /* Static Members */
    static Vector3 UNIT_X;
    static Vector3 UNIT_Y;
    static Vector3 UNIT_Z;
    static Vector3 ZERO;

    /* Operator Overload Functions */
    Vector3 operator+ (const Vector3 & other) const;
    Vector3 operator- () const;
    Vector3 operator- (const Vector3 & other) const;
    Vector3 operator* (const float & multiplicand) const;
    Vector3 operator/ (const float divisor) const;
    bool operator== (const Vector3 & other) const;
    bool operator!= (const Vector3 & other) const;

    /* Member Functions */
    float dot(const Vector3 & other) const;
    Vector3 cross(const Vector3 & other) const;
    float length() const;
    Vector3 normalized() const;
    void normalize();
    void print();
    Vector3 rotated(const Quaternion & quat) const;
    void rotate(const Quaternion & quat);

    /* Static Functions */
    static Vector3 lerp(const Vector3 & from, const Vector3 & to, float alpha);
};

/**
 * Vector4
 */
class Vector4 {
public:
    float x, y, z, w;
    Vector4(): x(0), y(0), z(0), w(0) {}
    Vector4(float w, float x, float y, float z): x(x), y(y), z(z), w(w) {}
    Vector4(const Vector4 & vec): x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}

    /* Static Members */
    static Vector4 UNIT_X;
    static Vector4 UNIT_Y;
    static Vector4 UNIT_Z;
    static Vector4 UNIT_W;
    static Vector4 ZERO;

    /* Operator Overload Functions */
    Vector4 operator+ (const Vector4 & other) const;
    Vector4 operator- () const;
    Vector4 operator- (const Vector4 & other) const;
    Vector4 operator* (const float & multiplicand) const;
    Vector4 operator/ (const float divisor) const;
    bool operator== (const Vector4 & other) const;
    bool operator!= (const Vector4 & other) const;

    /* Member Functions */
    float dot(const Vector4 & other) const;
    float length() const;
    Vector4 normalized() const;
    void normalize();
    void print();

    /* Static Functions */
    static Vector4 lerp(const Vector4 & from, const Vector4 & to, float alpha);
};


/**
 * Quaternion
 */
class Quaternion {
public:
    float x, y, z, w;
    Quaternion(): x(0), y(0), z(0), w(1) {}
    Quaternion(float x, float y, float z, float w): x(x), y(y), z(z), w(w) {}
    Quaternion(const Quaternion & quat): x(quat.x), y(quat.y), z(quat.z), w(quat.w) {}

    /* Static Members */
    static Quaternion IDENTITY;

    /* Operator Overload Functions */
    Quaternion operator* (const Quaternion & other) const;
    bool operator== (const Quaternion & other) const;
    bool operator!= (const Quaternion & other) const;

    /* Member Functions */
    float dot(const Quaternion & other) const;
    Quaternion normalized() const;
    void normalize();
    Vector3 toEulerAngles() const;
    Quaternion inverse() const;
    void print();

    /* Static Functions */
    static Quaternion lerp(const Quaternion & from, const Quaternion & to, float alpha);
    static Quaternion slerp(const Quaternion & from, const Quaternion & to, float alpha);
    static Quaternion fromEulerAngles(const Vector3 & vec);
    static Quaternion fromToRotation(const Vector3 & from, const Vector3 to);
    static Quaternion fromAxisAngle(const Vector3 & axis, float angle);
};

}

Lurdr::Vector2 operator* (float multiplier, const Lurdr::Vector2 & multiplicand);
Lurdr::Vector3 operator* (float multiplier, const Lurdr::Vector3 & multiplicand);
Lurdr::Vector4 operator* (float multiplier, const Lurdr::Vector4 & multiplicand);

#endif