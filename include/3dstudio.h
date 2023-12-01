/*
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>
#include <vector>
#include <array>

// Macro for index to buffer pointer conversion
#define BUFFER_OFFSET(i) (reinterpret_cast<char*>(0 + (i)))

// *************
// Debug macros

/*
 * Usage:
 * string s = "foo";
 * DEBUG( "Value of s is '" << s << "'");
 * -> DBG: Value of s is 'foo'
 */
#define DEBUG( msg ) \
    std::cerr << "DBG " << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl

/*
 * Usage:
 * string s = "foo";
 * DEBUGVAR( s );
 * -> DBG: testfile.cpp(20) s = foo
 */
#define DEBUGVAR( var )\
    std::cerr << "DBG " << __FILE__ << "(" << __LINE__ << "): " \
    << #var << " = " << (var) << std::endl

// *************
struct Vec3 {
    float v1, v2, v3;
public:
    constexpr
    Vec3 (const float x = 0, const float y = 0, const float z = 0)
        : v1{x}, v2{y}, v3{z}
    {}

    constexpr float x() const noexcept { return v1; };
    constexpr float y() const noexcept { return v2; };
    constexpr float z() const noexcept { return v3; };

    friend constexpr
    Vec3 operator+(const Vec3& a, const Vec3& b) {
        return Vec3(a.x() + b.x(),
                    a.y() + b.y(),
                    a.z() + b.z());
    }
    
    template<typename T>
    friend constexpr
    Vec3 operator+(const Vec3& a, const T& b) {
        return Vec3(a.x() + b,
                    a.y() + b,
                    a.z() + b);
    }
    
    template<typename T>
    friend constexpr
    Vec3 operator+(const T& b, const Vec3& a) {
        return Vec3(a.x() + b,
                    a.y() + b,
                    a.z() + b);
    }
    
    friend constexpr
    Vec3 operator*(const Vec3& a, const Vec3& b) {
        return Vec3(a.x() * b.x(),
                    a.y() * b.y(),
                    a.z() * b.z());
    }
    
    template<typename T>
    friend constexpr
    Vec3 operator*(const Vec3& a, const T& b) {
        return Vec3(a.x() * b,
                    a.y() * b,
                    a.z() * b);
    }
    
    template<typename T>
    friend constexpr
    Vec3 operator*(const T& b, const Vec3& a) {
        return Vec3(a.x() * b,
                    a.y() * b,
                    a.z() * b);
    }
    
    friend
    float dot(const Vec3& a, const Vec3& b) {
        Vec3 tmp(a * b);
        return (tmp.v1 + tmp.v2 + tmp.v3);
    }

    float length() const {
        Vec3 tmp(v1*v1, v2*v2, v3*v3);
        return sqrt(tmp.v1 + tmp.v2 + tmp.v3);
    }
    
    void normalize() {
        float len = this->length();
        this->v1 /= len;
        this->v2 /= len;
        this->v3 /= len;
    }

    Vec3 normalized() const {
        float len = this->length();
        return Vec3(v1/len, v2/len, v3/len);
    }

};


struct Vec4 {
    float v1, v2, v3, v4;
public:
    constexpr
    Vec4 (const float x = 0, const float y = 0, const float z = 0, const float w = 0)
        : v1{x}, v2{y}, v3{z}, v4{w}
    {}

    constexpr float x() const noexcept { return v1; };
    constexpr float y() const noexcept { return v2; };
    constexpr float z() const noexcept { return v3; };
    constexpr float w() const noexcept { return v4; };

    constexpr
    Vec3 toVector3D() const noexcept {
        return Vec3(v1, v2, v3);
    }
    
    friend constexpr
    Vec4 operator+(const Vec4& a, const Vec4& b) {
        return Vec4(a.x() + b.x(),
                    a.y() + b.y(),
                    a.z() + b.z(),
                    a.w() + b.w());
    }
    
    template<typename T>
    friend constexpr
    Vec4 operator+(const Vec4& a, const T& b) {
        return Vec4(a.x() + b,
                    a.y() + b,
                    a.z() + b,
                    a.w() + b);
    }
    
    template<typename T>
    friend constexpr
    Vec4 operator+(const T& b, const Vec4& a) {
        return Vec4(a.x() + b,
                    a.y() + b,
                    a.z() + b,
                    a.w() + b);
    }
    
    friend constexpr
    Vec4 operator*(const Vec4& a, const Vec4& b) {
        return Vec4(a.x() * b.x(),
                    a.y() * b.y(),
                    a.z() * b.z(),
                    a.w() * b.w());
    }
    
    template<typename T>
    friend constexpr
    Vec4 operator*(const Vec4& a, const T& b) {
        return Vec4(a.x() * b,
                    a.y() * b,
                    a.z() * b,
                    a.w() * b);
    }
    
    template<typename T>
    friend constexpr
    Vec4 operator*(const T& b, const Vec4& a) {
        return Vec4(a.x() * b,
                    a.y() * b,
                    a.z() * b,
                    a.w() * b);
    }
    
    friend
    float dot(const Vec4& a, const Vec4& b) {
        Vec4 tmp(a * b);
        return tmp.x() + tmp.y() + tmp.z() + tmp.w();
    }

    float length() const {
        Vec4 tmp(v1*v1, v2*v2, v3*v3, v4*v4);
        return sqrt(tmp.v1 + tmp.v2 + tmp.v3 + tmp.v4);
    }
    
    void normalize() {
        float len = this->length();
        this->v1 /= len;
        this->v2 /= len;
        this->v3 /= len;
        this->v4 /= len;
    }

    Vec4 normalized() const {
        float len = this->length();
        return Vec4(v1/len, v2/len, v3/len, v4/len);
    }
};

// The matrix is stored in row-major form
struct Mat4x4 {
    std::array<float,16> matrix;
public:
    Mat4x4 (const float m11 = 0, const float m12 = 0, const float m13 = 0, const float m14 = 0,
            const float m21 = 0, const float m22 = 0, const float m23 = 0, const float m24 = 0,
            const float m31 = 0, const float m32 = 0, const float m33 = 0, const float m34 = 0,
            const float m41 = 0, const float m42 = 0, const float m43 = 0, const float m44 = 0 )
        : matrix{m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,m41,m42,m43,m44}
    {}

    constexpr
    const float* data() noexcept
    {
        return matrix.data();
    }

    void setToIdentity () noexcept
    {
        matrix.fill(0);
        matrix[0] = 1.0f;
        matrix[5] = 1.0f;
        matrix[10] = 1.0f;
        matrix[15] = 1.0f;
    }

    void print() const
    {
        for(int i=0;i<16;++i) {
            std::cout << matrix[i] << " ";
            if (!fmod(i+1,4))
                std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    friend
    Mat4x4 operator*(const Mat4x4 A, const Mat4x4 B)
    {
        Mat4x4 M;

        for (int r=0; r<4; ++r) {
            for (int c=0; c<4; ++c) {
                float v = 0;
                for (int i=0; i<4; ++i) {
                    v += A.matrix[(r*4)+i]*B.matrix[c+(i*4)];
                }
                M.matrix[(r*4)+c] = v;
            }
        }

        return M;
    }


};




