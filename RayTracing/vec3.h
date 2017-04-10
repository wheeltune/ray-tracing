//
//  vec3.h
//  RayTracing
//
//  Created by wheeltune on 10.04.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef vec3_h
#define vec3_h

#include <iostream>

#include "geometry_constants.h"

namespace Geometry {
    struct Vec3 {
        long double vec[3];
        
        Vec3(long double v1, long double v2, long double v3);
        Vec3(long double v) : Vec3(v, v, v) { };
        Vec3() { }
        
        Vec3 limit(long double down, long double up);
        
        long double& operator[] (int index);
        const long double operator[]  (int index) const;
        
        Vec3& operator +=(const Vec3& v);
        Vec3& operator *=(const Vec3& v);
        Vec3& operator -=(const Vec3& v);
        Vec3& operator /=(const Vec3& v);
        
        Vec3 operator -();
        Vec3 operator +();
        
        Vec3& operator *=(float k);
        Vec3& operator /=(float k);
    };
    
    Vec3::Vec3(long double v1, long double v2, long double v3) {
        vec[0] = v1;
        vec[1] = v2;
        vec[2] = v3;
    }
    
    long double& Vec3::operator[] (int index) {
        return vec[index];
    }
    
    const long double Vec3::operator[] (int index) const {
        return vec[index];
    }
    Vec3& Vec3::operator+=(const Vec3& v) {
        for (int i = 0; i < 3; ++i)
            (*this)[i] += v[i];
        return *this;
    }
    
    Vec3& Vec3::operator-=(const Vec3& v) {
        for (int i = 0; i < 3; ++i)
            (*this)[i] -= v[i];
        return *this;
    }
    
    Vec3& Vec3::operator*=(float k) {
        for (int i = 0; i < 3; ++i)
            (*this)[i] *= k;
        return *this;
    }
    
    Vec3& Vec3::operator*=(const Vec3& v) {
        for (int i = 0; i < 3; ++i)
            (*this)[i] *= v[i];
        return *this;
    }
    
    Vec3& Vec3::operator/=(float k) {
        for (int i = 0; i < 3; ++i)
            (*this)[i] /= k;
        return *this;
    }
    
    Vec3& Vec3::operator/=(const Vec3& v) {
        for (int i = 0; i < 3; ++i)
            (*this)[i] /= v[i];
        return *this;
    }
    
    Vec3 operator +(Vec3 p1, const Vec3& p2) {
        return p1 += p2;
    }
    
    Vec3 operator -(Vec3 p1, const Vec3& p2) {
        return p1 -= p2;
    }
    
    Vec3 operator *(Vec3 p1, const Vec3& p2) {
        return p1 *= p2;
    }
    
    Vec3 operator *(Vec3 p, float a) {
        return p *= a;
    }
    
    Vec3 operator *(float a, Vec3 p) {
        return p *= a;
    }
    
    Vec3 operator /(Vec3 p1, const Vec3& p2) {
        return p1 /= p2;
    }
    
    Vec3 operator /(Vec3 p, float a) {
        return p /= a;
    }
    
    Vec3 Vec3::operator -() {
        return Vec3(*this) * (-1);
    }
    
    Vec3 Vec3::operator+() {
        return Vec3(*this);
    }
    
    
    Vec3 Vec3::limit(long double down, long double up) {
        Vec3 ans;
        for (int i = 0; i < 3; ++i) {
            ans[i] = std::max(std::min(vec[i], up), down);
        }
        return ans;
    }
}

#endif /* vec3_h */
